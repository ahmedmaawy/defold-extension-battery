package com.defold.Battery;

import android.os.Environment;
import android.os.BatteryManager;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.Context;

import java.lang.Math;

public class Battery
{
	// Return the battery level as a float between 0 and 1 (1 being fully charged, 0 fulled discharged)
    public static int GetBatteryPct(Context context)
    {
    	Intent batteryStatus = GetBatteryStatusIntent(context);

    	int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
		int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);

		int batteryPct = Math.round(level * 100 / (float)scale);
		return batteryPct;
    }

    // Return whether or not we're currently on charge
    public static boolean IsBatteryCharging(Context context)
    {
    	Intent batteryStatus = GetBatteryStatusIntent(context);

    	int status = batteryStatus.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
    	return status == BatteryManager.BATTERY_STATUS_CHARGING ||
            status == BatteryManager.BATTERY_STATUS_FULL;
    }

    // Battery status intent
    private static Intent GetBatteryStatusIntent(Context context)
    {
 		IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		return context.registerReceiver(null, ifilter);
    }
}