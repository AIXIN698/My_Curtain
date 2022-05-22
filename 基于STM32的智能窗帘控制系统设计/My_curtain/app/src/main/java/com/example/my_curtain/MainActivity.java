package com.example.my_curtain;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.my_curtain.util.ToastUtil;

public class MainActivity extends AppCompatActivity {

    private LinearLayout mbj_1;
    private Button mbtn_1;
    private EditText med_1;
    private EditText med_2;
    private TextView mtv_1;
    public static String username = "AIXIN698";
    public static String password = "123456";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        /* ------------------------------------------------------------------------------------- */
        // 隐藏状态栏方法
        View decorView = getWindow().getDecorView();
        int option = View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
        decorView.setSystemUiVisibility(option);
        getWindow().setStatusBarColor(Color.TRANSPARENT);
        // 隐藏标题栏
        ActionBar actionBar = getSupportActionBar();
        assert actionBar != null;
        actionBar.hide();
        /* ------------------------------------------------------------------------------------- */

        /* ------------------------------ 控件实列化 ---------------------------------------------*/
        mbj_1 = findViewById(R.id.main_bj);
        mbj_1.getBackground().setAlpha(150);//0~255透明度值

        med_1 = findViewById(R.id.main_ed_1);
        med_2 = findViewById(R.id.main_ed_2);
        mbtn_1 = findViewById(R.id.main_btn_1);
        mtv_1 = findViewById(R.id.main_tv_1);

        /* ------------------------------------------------------------------------------------- */

        // 登录控件点击事件
        mbtn_1.setOnClickListener(this::onClick);
        mtv_1.setOnClickListener(this::wjmm);
    }

    private void wjmm(View v){

        Intent intent = null;
        intent = new Intent(MainActivity.this, c_password_Activity.class);
        startActivity(intent);

    }

    private void onClick(View v){

        // 弹窗内容设置
        String ok = "登录成功";
        String fail = "用户名或密码输入错误，请重新输入!";
        String sul = "请先输入用户名与密码!";

        // 获取输入的用户名和密码
        String username_1 = med_1.getText().toString();   // 将控件mEtUser中的数据提取出来，转换成字符串赋值给username
        String password_1 = med_2.getText().toString();

        // 判断如果账户名与密码为“AIXIN698”&&“123456”则执行界面跳转
        if((username_1.equals(username))&&(password_1.equals(password))){
            ToastUtil.showmsg(MainActivity.this,ok);
            Intent intent = null;
            intent = new Intent(MainActivity.this, direct_Activity.class);
            startActivity(intent);
        }else if(username.equals("") && password.equals("")){
            ToastUtil.showmsg(MainActivity.this,sul);
        }
        else{
            ToastUtil.showmsg(MainActivity.this,fail);
        }

    }

}