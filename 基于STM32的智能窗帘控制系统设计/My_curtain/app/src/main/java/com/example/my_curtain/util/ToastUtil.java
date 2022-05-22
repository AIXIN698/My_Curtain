package com.example.my_curtain.util;

import android.content.Context;
import android.widget.Toast;

public class ToastUtil {
    public static Toast mToast;
    public static void showmsg(Context context, String msg){
        if(mToast == null){
            mToast = Toast.makeText(context, msg, Toast.LENGTH_SHORT);
        }else{
            mToast.setText(msg);
        }
        mToast.show();
    }
}
