﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

using ControlLibraryS8_53;
using LibraryS8_53;


/*
 * Алгоритм работы
 * 1. Дать команду DISPLAY:AUTOSEND
 * 2. Запустить поток чтения порта. Принять последовательность байт. Признаком конца приёма является промежуток молчания более нескольких секунд.
 * 3. Проверить, что поледним байтом является команда END_SCENE.
 * 4. И, если п.3 выполняется, выполнить последовательность байт.
 * 5. Если есть команды органов управления, выполнить их передачу в прибор.
 * 6. Перейти к п.1
 * */

namespace S8_53_USB {

    public partial class MainForm : Form {

        private class Commands
        {
            public void Push(String command)
            {
                mutex.WaitOne();
                commands.Enqueue(command);
                mutex.ReleaseMutex();
            }
            public String Pop()
            {
                String result = "";
                mutex.WaitOne();
                if(commands.Count != 0)
                {
                    result = commands.Dequeue();
                }
                mutex.ReleaseMutex();
                return result;
            }
            private static Queue<string> commands = new Queue<string>();
            private static Mutex mutex = new Mutex();
        };

        private static Commands commands = new Commands();

        // Этот порт используется для соединения по USB
        private static LibraryS8_53.ComPort port = new LibraryS8_53.ComPort();

        // Этот сокет используется для соединения по LAN
        private static LibraryS8_53.SocketTCP socket = new LibraryS8_53.SocketTCP();

        private Dictionary<Button, string> mapButtons = new Dictionary<Button, string>();

        // Сюда будем считывать данные из порта
        private static Queue<byte> data = new Queue<byte>();

        // Признак того, что нужно отключиться от портов при первой возможности
        private static bool needForDisconnect = false;

        // Будет использоваться для чтения данных из VCP
        private BackgroundWorker readerUSB = new BackgroundWorker();

        // Будет использоваться для чтения данных из LAN
        private BackgroundWorker readerLAN = new BackgroundWorker();

        // Если true, нужно посылать запрос на следующий фрейм
        private static bool needAutoSend2 = false;

        private enum Command : byte
        {
            EMPTY_FUNCTION = 0,


            SET_PALETTE_COLOR = 3,
            SET_COLOR = 4,
            SET_FONT = 5,
            DRAW_TEXT = 6,
            DRAW_PIXEL = 7,

            FILL_REGION = 9,
            DRAW_MULTI_HPOINT_LINE = 10,
            DRAW_MULTI_VPOINT_LINE = 11,
            DRAW_SIGNAL_LINES = 12,
            DRAW_SIGNAL_POINTS = 13,
            DRAW_VLINES_ARRAY = 14,
            INVALIDATE = 15,



            DRAW_HLINE = 19,
            DRAW_VLINE = 20,
            LOAD_FONT = 22,
            NUM_COMMANDS = 23
        };

        public MainForm() {
            InitializeComponent();

            mapButtons.Add(btnChannel0,    "CHAN1");
            mapButtons.Add(btnService,     "SERVICE");
            mapButtons.Add(btnChannel1,    "CHAN2");
            mapButtons.Add(btnDisplay,     "DISPLAY");
            mapButtons.Add(btnTime,        "TIME");
            mapButtons.Add(btnMemory,      "MEMORY");
            mapButtons.Add(btnTrig,        "TRIG");
            mapButtons.Add(btnStart,       "START");
            mapButtons.Add(btnCursors,     "CURSORS");
            mapButtons.Add(btnMeasures,    "MEASURES");
            mapButtons.Add(btnHelp,        "HELP");
            mapButtons.Add(btnMenu,        "MENU");
            mapButtons.Add(btnF1,          "1");
            mapButtons.Add(btnF2,          "2");
            mapButtons.Add(btnF3,          "3");
            mapButtons.Add(btnF4,          "4");
            mapButtons.Add(btnF5,          "5");

            buttonUpdatePorts_Click(null, null);

            buttonConnectLAN.Enabled = true;

            readerUSB.DoWork += ReaderUSB_DoWork;
            readerUSB.RunWorkerCompleted += ReaderUSB_Completed;

            readerLAN.DoWork += ReaderLAN_DoWork;
            readerLAN.RunWorkerCompleted += ReaderLAN_Completed;
        }

        private void button_MouseDown(object sender, MouseEventArgs args) {
            if ((Button)sender != buttonConnectUSB)
            {
                try
                {
                    commands.Push("KEY:" + StringToSendForButton(sender) + " DOWN");
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
            }
        }

        private void button_MouseUp(object sender, MouseEventArgs args) {
            if ((Button)sender != buttonConnectUSB)
            {
                try
                {
                    commands.Push("KEY:" + StringToSendForButton(sender) + " UP");
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
            }
        }

        private void governor_RotateLeft(object sender, EventArgs args) {
            try
            {
                commands.Push("GOV:" + ((Governor)sender).ValueToSend + " LEFT");
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private void governor_RotateRight(object sender, EventArgs args) {
            try
            {
                commands.Push("GOV:" + ((Governor)sender).ValueToSend + " RIGHT");
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private string StringToSendForButton(object btn) {
            string retValue = "";
            
            try
            {
                retValue = mapButtons[(Button)btn];
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            return retValue;
        }

        private void buttonUpdatePorts_Click(object sender, EventArgs args) {
            try
            {
                string[] ports = port.GetPorts();
                comboBoxPorts.Items.Clear();
                comboBoxPorts.Items.AddRange(ports);
                comboBoxPorts.SelectedIndex = ports.Length - 1;
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private void buttonConnectUSB_Click(object sender, EventArgs e)
        {
            if (port.IsOpen())                                  // Если порт открыт - идёт обмен с прибором. Будем отключать
            {
                needForDisconnect = true;
                buttonConnectUSB.Text = "Подкл";
                comboBoxPorts.Enabled = true;
                buttonUpdatePorts.Enabled = true;
                textBoxIP.Enabled = true;
                textBoxPort.Enabled = true;
                buttonConnectLAN.Enabled = true;
            }
            else
            {
                if (port.Connect(comboBoxPorts.SelectedIndex, false)) // иначе делаем попыткую подключиться
                {
                    needForDisconnect = false;
                    textBoxIP.Enabled = false;
                    textBoxPort.Enabled = false;
                    buttonConnectLAN.Enabled = false;
                    comboBoxPorts.Enabled = false;
                    buttonUpdatePorts.Enabled = false;
                    buttonConnectUSB.Text = "Откл";
                    port.SendString("DISPLAY:AUTOSEND 1");
                    readerUSB.RunWorkerAsync();
                }
            }
        }

        private void ReaderUSB_DoWork(object sender, DoWorkEventArgs args)
        {
            byte[] bytes = port.ReadBytes();
            for(int i = 0; i < bytes.Length; i++)
            {
                data.Enqueue(bytes[i]);
            }
        }

        private static long prev_time = 0;

        private void ReaderUSB_Completed(object sender, RunWorkerCompletedEventArgs args)
        {
            if (data.Count != 0)
            {
                byte[] bytes = data.ToArray();

                if (bytes[data.Count - 1] == (byte)Command.INVALIDATE)
                {
                    prev_time = CurrentTime();

                    RunData();
                }
                else
                {
                    if (port.IsOpen() && CurrentTime() - prev_time > 100)
                    {
                        needAutoSend2 = true;
                    }
                }
            }
            else
            {
                if(port.IsOpen() && CurrentTime() - prev_time > 100)
                {
                    needAutoSend2 = true;
                }
            }

            String command = commands.Pop();
            if (command != "")
            {
                port.SendString(command);
            }

            if (needForDisconnect)
            {
                port.Stop();
                needForDisconnect = false;
                textBoxIP.Enabled = true;
                textBoxPort.Enabled = true;
                buttonConnectLAN.Enabled = true;
                comboBoxPorts.Enabled = true;
                buttonUpdatePorts.Enabled = true;
                buttonConnectUSB.Text = "Подкл";
            }
            else
            {
                if (needAutoSend2)
                {
                    needAutoSend2 = false;
                    port.SendString("DISPLAY:AUTOSEND 2");
                }

                readerUSB.RunWorkerAsync();
            }
        }

        private void ReaderLAN_Completed(object sender, RunWorkerCompletedEventArgs args)
        {
            if (data.Count != 0)
            {
                byte[] bytes = data.ToArray();

                if (bytes[data.Count - 1] == (byte)Command.INVALIDATE)
                {
                    RunData();
                }
            }

            if (needForDisconnect)
            {
                socket.Disconnect();
                needForDisconnect = false;
                textBoxIP.Enabled = true;
                textBoxPort.Enabled = true;
                buttonConnectLAN.Enabled = true;
                comboBoxPorts.Enabled = true;
                buttonUpdatePorts.Enabled = true;
                buttonConnectUSB.Text = "Подкл";
                buttonConnectLAN.Text = "Подкл";
            }
            else
            {
                if (needAutoSend2)
                {
                    String command = commands.Pop();
                    if(command != "")
                    {
                        socket.SendString(command);
                        command = commands.Pop();
                    }

                    needAutoSend2 = false;
                    socket.SendString("DISPLAY:AUTOSEND 2");
                }

                readerLAN.RunWorkerAsync();
            }
        }


        private void buttonConnectLAN_Click(object sender, EventArgs e)
        {
            try
            {
                if(socket.IsConnected())                                            // Проверяем, установлено ли уже соединение, и если да
                {
                    needForDisconnect = true;
                    buttonConnectLAN.Text = "Подкл";
                    textBoxIP.Enabled = true;
                    textBoxPort.Enabled = true;
                    buttonUpdatePorts.Enabled = true;
                    EnableControlsUSB(true);
                }
                else                                                                // А по этой ветке подключаемся
                {
                    if(socket.Connect(textBoxIP.Text, Int32.Parse(textBoxPort.Text)))
                    {
                        needForDisconnect = false;
                        buttonConnectLAN.Text = "Откл";
                        textBoxIP.Enabled = false;
                        textBoxPort.Enabled = false;
                        comboBoxPorts.Enabled = false;
                        buttonUpdatePorts.Enabled = false;
                        buttonConnectUSB.Enabled = false;
                        socket.Clear();
                        socket.SendString("DISPLAY:AUTOSEND 1");
                        readerLAN.RunWorkerAsync();
                    }
                }
            }
            catch(Exception)
            {

            }
        }

        private void ReaderLAN_DoWork(object sender, DoWorkEventArgs args)
        {
            int size_before = data.Count;

            byte[] bytes = socket.ReadBytes(4);
            if (bytes.Length != 0)
            {
                for (int i = 0; i < bytes.Length; i++)
                {
                    data.Enqueue(bytes[i]);
                }
            }
        }

        private void cbPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxPorts.Enabled)
            {
                buttonConnectUSB.Enabled = port.DeviceConnectToPort(comboBoxPorts.SelectedIndex);
            }
        }

        private void MainForm_Closed(object sender, EventArgs e)
        {
            needForDisconnect = true;
            port.Stop();
            socket.Disconnect();
            while (port.IsOpen() || socket.IsConnected()) { };
        }

        // Активировать/деактивировать элементы управления, отвечающие за связь по USB
       private void EnableControlsUSB(bool enable)
        {
            if (enable)
            {
                cbPorts_SelectedIndexChanged(null, null);
            }
            comboBoxPorts.Enabled = enable;
            buttonUpdatePorts.Enabled = enable;
        }

        // Выполнить имеющиеся данные
        private static void RunData()
        {
            try
            {
                while (data.Count != 0)
                {
                    byte command = (byte)int8();

                    if ((Command)command == Command.SET_COLOR)
                    {
                        Display.SetColor((uint)int8());
                    }
                    else if ((Command)command == Command.SET_PALETTE_COLOR)
                    {
                        Display.SetPalette((byte)int8(), (ushort)int16());
                    }
                    else if ((Command)command == Command.FILL_REGION)
                    {
                        Display.FillRegion(int16(), int8(), int16(), int8());
                    }
                    else if ((Command)command == Command.INVALIDATE)
                    {
                        // Выводим нарисованную картинку
                        Display.EndScene();
                        needAutoSend2 = true;
                    }
                    else if ((Command)command == Command.DRAW_HLINE)
                    {
                        Display.DrawHLine(int8(), int16(), int16());
                    }
                    else if ((Command)command == Command.DRAW_VLINE)
                    {
                        Display.DrawVLine(int16(), int8(), int8());
                    }
                    else if ((Command)command == Command.DRAW_PIXEL)
                    {
                        Display.SetPoint(int16(), int8());
                    }
                    else if ((Command)command == Command.DRAW_SIGNAL_POINTS)
                    {
                        int x0 = int16();

                        for (int i = 0; i < 281; i++)
                        {
                            Display.SetPoint(x0 + i, int8());
                        }
                    }
                    else if ((Command)command == Command.DRAW_SIGNAL_LINES)
                    {
                        int x0 = int16();

                        int prevX = int8();

                        for (int i = 0; i < 280; i++)
                        {
                            int nextX = int8();
                            Display.DrawVLine(x0 + i, prevX, nextX);
                            prevX = nextX;
                        }
                    }
                    else if ((Command)command == Command.DRAW_MULTI_HPOINT_LINE)
                    {
                        int numLines = int8();
                        int x0 = int16();
                        int numPoints = int8();
                        int dX = int8();
                        for (int i = 0; i < numLines; i++)
                        {
                            int y = int8();

                            for (int point = 0; point < numPoints; point++)
                            {
                                Display.SetPoint(x0 + dX * point, y);
                            }
                        }
                    }
                    else if ((Command)command == Command.DRAW_MULTI_VPOINT_LINE)
                    {
                        int numLines = int8();
                        int y0 = int8();
                        int numPoints = int8();
                        int dY = int8();
                        int8();
                        for (int i = 0; i < numLines; i++)
                        {
                            int x = int16();

                            for (int point = 0; point < numPoints; point++)
                            {
                                Display.SetPoint(x, y0 + dY * point);
                            }
                        }
                    }
                    else if ((Command)command == Command.DRAW_VLINES_ARRAY)
                    {
                        int x0 = int16();
                        int numLines = int8();
                        for (int i = 0; i < numLines; i++)
                        {
                            Display.DrawVLine(x0 + i, int8(), int8());
                        }
                    }
                    else if ((Command)command == Command.LOAD_FONT)
                    {
                        int typeFont = int8();
                        if (typeFont < 4)
                        {
                            Display.fonts[typeFont] = new Display.MyFont();
                            Display.fonts[typeFont].height = int8();
                            int8();
                            int8();
                            int8();
                            for (int i = 0; i < 256; i++)
                            {
                                Display.fonts[typeFont].symbols[i] = new Display.Symbol();
                                Display.fonts[typeFont].symbols[i].width = int8();
                                for (int j = 0; j < 8; j++)
                                {
                                    Display.fonts[typeFont].symbols[i].bytes[j] = int8();
                                }
                            }
                        }
                    }
                    else if ((Command)command == Command.SET_FONT)
                    {
                        Display.currentFont = int8();
                    }
                    else if ((Command)command == Command.DRAW_TEXT)
                    {
                        int x0 = int16();
                        int y0 = int8();
                        int numSymbols = int8();
                        char[] str = new char[numSymbols];
                        for (int i = 0; i < numSymbols; i++)
                        {
                            str[i] = (char)int8();
                        }
                        Display.DrawText(x0, y0, str);
                    }
                    else
                    {
                        Console.WriteLine("Неизвестная команда " + command);
                    }
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private static int int8()
        {
            return data.Dequeue(); ;
        }

        private static int int16()
        {
            return int8() + (int8() << 8);
        }

        private static long CurrentTime()
        {
            return DateTime.Now.Ticks / 10000;
        }
    }
}
