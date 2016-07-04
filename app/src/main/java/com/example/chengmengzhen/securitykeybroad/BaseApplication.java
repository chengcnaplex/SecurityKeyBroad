package com.example.chengmengzhen.securitykeybroad;

import android.app.Application;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;


public class BaseApplication extends Application
{

	private static Context	mContext;
	private static Handler	mHandler;
	private static Thread	mMainThread;
	private static long		mMainThreadId;
	private static Looper	mMainThreadLooper;

	@Override
	public void onCreate()
	{
		super.onCreate();

		// 程序的入口
		mContext = this;

		// handler,用来子线程和主线程通讯
		mHandler = new Handler();

		// 主线程
		mMainThread = Thread.currentThread();
		// id
		// mMainThreadId = mMainThread.getId();
		mMainThreadId = android.os.Process.myTid();

		// looper
		mMainThreadLooper = getMainLooper();
	}

	public static Context getContext()
	{
		return mContext;
	}

	public static Handler getHandler()
	{
		return mHandler;
	}

	public static Thread getMainThread()
	{
		return mMainThread;
	}

	public static long getMainThreadId()
	{
		return mMainThreadId;
	}

	public static Looper getMainThreadLooper()
	{
		return mMainThreadLooper;
	}

}
