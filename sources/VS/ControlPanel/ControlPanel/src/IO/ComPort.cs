﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.IO.Ports;

namespace LibraryS8_53
{
    public class EventArgsReceiveComPort : EventArgs
    {
        public string data;

        public EventArgsReceiveComPort(string data)
        {
            this.data = data;
        }
    }

    public class ComPort
    {
        private static SerialPort port;
        private static string[] ports;

        public ComPort()
        {
            port = new SerialPort();
            port.ReadTimeout = 100;
            port.BaudRate = 125000;
            // Устанавливаем количество байт в приёмном буфере, при котором будет вызываться обработчик приёма
            port.ReceivedBytesThreshold = 1;
        }

        public void Stop()
        {
            port.Close();
        }

        public string[] GetPorts()
        {
            ports = SerialPort.GetPortNames();
            return ports;
        }

        public bool DeviceConnectToPort(int numPort)
        {
            if(port.IsOpen)
            {
                port.Close();
            }
            port.PortName = ports[numPort];
            string answer;
            try
            {
                port.Open();
                if (port.IsOpen)
                {
                    SendString("REQUEST ?");
                    answer = ReadLine();
                    port.Close();
                    bool retValue = (answer == "S8-54");
                    return retValue;
                }
            }
            catch (SystemException)
            {
                port.Close();
            }
            return false;
        }

        // Прочитать данные из буфера. Данные будут считываться до тех пор, пока не пройдёт timeWaitMS с момента приёма последнего байта
        public byte[] ReadBytes(long timeWaitMS)
        {
            long timeLast = CurrentTime();
            int numBytesPrev = 0;
            while(CurrentTime() - timeLast < timeWaitMS)
            {
                if(port.BytesToRead != numBytesPrev)
                {
                    timeLast = CurrentTime();
                    numBytesPrev = port.BytesToRead;
                }
            }

            int numBytes = port.BytesToRead;
            byte[] bytes = new byte[numBytes];
            port.Read(bytes, 0, numBytes);

            return bytes;
        }

        public void SendString(string str)
        {
            if (port.IsOpen)
            {
                while (port.BytesToWrite != 0) { };
                port.Write(":" + str + "\x0d");
            }
        }

        static public string ReadLine()
        {
            string line;
            try
            {
                line = port.ReadLine();
                return line.Substring(0, line.Length - 1);
            }
            catch (SystemException)
            {

            }
            return null;
        }

        public bool Connect(int numPort, bool handlerEnable)
        {
            try
            {
                port.PortName = ports[numPort];
                port.Open();
                if (port.IsOpen)
                {
                    SendString("REQUEST ?");
                    string answer = ReadLine();
                    if (answer != "S8-54")
                    {
                        port.Close();
                    }
                }
            }
            catch (SystemException)
            {
                port.Close();
            }
            return port.IsOpen;
        }

        private static byte[] data = new byte[16 * 1024];

        public SerialPort GetSerialPort()
        {
            return port;
        }

        public bool IsOpen()
        {
            return port.IsOpen;
        }

        private static long CurrentTime()
        {
            return DateTime.Now.Ticks / 10000;
        }
    }
}
