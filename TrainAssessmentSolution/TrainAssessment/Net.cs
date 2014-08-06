using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;



namespace TrainAssessment
{
    public class Net
    {
        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void EnableAcceptor();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void SetListenPort(int port);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void SetMaxReadUnit(int mru);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void ConnectServer(StringBuilder addr, StringBuilder port, int sn, int timeout, bool reconnect);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void CloseConnection(int fd, int sn);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern bool Start();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern bool Stop();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void PacketReset();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern bool PickPacket(ref int fd, ref int sn, ref int code, ref int err);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void SendPacket();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern char ReadByte();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern int ReadInt();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern float ReadFloat();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern string ReadString();

        [DllImport("ClientNet.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReadBytes(StringBuilder value, int size);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void CreateSendPacket(int fd,int sn, int code,int err);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void WriteByte(char value);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void WriteInt(int value);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void WriteFloat(float value);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void WriteString(StringBuilder value);

        [DllImport("ClientNet.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void WriteBytes(byte[] value, int size);

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern int GetWritePos();

        [DllImport("ClientNet.dll",CallingConvention=CallingConvention.Cdecl)]
        public static extern void SetWritePos(int pos);
    }
}
