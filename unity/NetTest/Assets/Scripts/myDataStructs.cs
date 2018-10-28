﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class myDataStructs : MonoBehaviour
{
    [StructLayout(LayoutKind.Sequential, Size = 20), System.Serializable]
    public struct BloidData
    {
        [MarshalAsAttribute(UnmanagedType.I4, SizeConst = 4)]
        public int objectId;
        [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
        public float x;
        [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
        public float y;
        [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
        public float z;
        [MarshalAsAttribute(UnmanagedType.I4, SizeConst = 4)]
        public int direction;

    };
    [DllImport("BoidEvents")]
    public static extern void raknetPeer();

    [DllImport("BoidEvents")]
    public static extern void connectToServer([MarshalAs(UnmanagedType.LPStr)]string ip);

    [DllImport("BoidEvents")]
    public static extern BloidData receiveData();

    [DllImport("BoidEvents")]
    public static extern void sendData(int id, float x, float y, float z, int dir);

    [DllImport("BoidEvents")]
    public static extern BloidData Test();

    [DllImport("BoidEvents")]
    public static extern BloidData InitialData();
}
