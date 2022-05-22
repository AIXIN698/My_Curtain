package com.example.my_curtain;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.example.my_curtain.util.ToastUtil;

public class c_password_Activity extends AppCompatActivity {

    private EditText med_1;
    private EditText med_2;
    private Button mbtn_1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cpassword);

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

        /* ------------------------------------------------------------------------------------- */

        med_1 = findViewById(R.id.pass_ed_1);
        med_2 = findViewById(R.id.pass_ed_2);
        mbtn_1 = findViewById(R.id.pass_btn_1);

        /* ------------------------------------------------------------------------------------- */

        mbtn_1.setOnClickListener(this::change_password);
    }

    private void change_password(View v){

        // 获取输入修改的用户名和密码
        String username_2 = med_1.getText().toString();   // 将控件mEtUser中的数据提取出来，转换成字符串赋值给username
        String password_2 = med_2.getText().toString();

        if(!username_2.equals("")&&!password_2.equals("")){
            MainActivity.username = username_2;
            MainActivity.password = password_2;

            ToastUtil.showmsg(c_password_Activity.this, "密码修改成功");
            Intent intent = null;
            intent = new Intent(c_password_Activity.this, MainActivity.class);
            startActivity(intent);
        }
        else if(username_2.length() != 6){
            ToastUtil.showmsg(c_password_Activity.this, "请输入6位数用户名");
        }
        else{
            ToastUtil.showmsg(c_password_Activity.this, "请输入密码");
        }

    }

}