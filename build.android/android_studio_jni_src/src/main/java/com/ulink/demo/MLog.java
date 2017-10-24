package com.example.hellolibs;

public class MLog {
	private static boolean IsDebug = true;
	private static String TAG = "VLINKS";
	public static void d(String msg)
	{
		if(IsDebug)
		{
			//Log.d(TAG, msg);
			System.out.println(msg);
		}
	}
}
