package com.example.my_curtain;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;

import com.example.my_curtain.util.ToastUtil;
import com.synnapps.carouselview.CarouselView;
import com.synnapps.carouselview.ImageListener;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

// 智能窗帘控制系统主界面
public class direct_Activity extends AppCompatActivity {

    /* ------------------------------------------------------------------------------------------ */
    private String host = "tcp://test.ranye-iot.net";
    private String userName = "android";
    private String passWord = "android";
    private String mqtt_id = "1353023461"; //定义成自己的QQ号  切记！不然会掉线！！！
    private String mqtt_sub_topic = "1353023461"; //为了保证你不受到别人的消息  哈哈    // APP接收消息端
    private String mqtt_pub_topic = "1353023461_ESP";//为了保证你不受到别人的消息  哈哈  自己QQ好后面加 _PC   // APP发送消息  （下位机端）
    private MqttConnectOptions options;
    private ScheduledExecutorService scheduler;
    private MqttClient client;
    private Handler handler;
    private int led_flag;
    private int curtain_flag;
    /* ------------------------------------------------------------------------------------------ */

    // 轮播图声明
    private CarouselView car_1;
    int[] images = {
            R.drawable.bj_1,
            R.drawable.bj_2,
            R.drawable.bj_3,
            R.drawable.bj_4,
    };
    private ImageButton mbtn_1;
    private ImageButton mbtn_2;
    private TextView mtv_1;
    private TextView mtv_2;
    private TextView mtv_3;
    private TextView mtv_4;
    private TextView mtv_5;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_direct);

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
        // 控件实列化
        mbtn_1 = findViewById(R.id.dir_btn_1);
        mbtn_2 = findViewById(R.id.dir_btn_2);
        mtv_1 = findViewById(R.id.dir_tv_1);
        mtv_2 = findViewById(R.id.dir_tv_2);
        mtv_3 = findViewById(R.id.dir_tv_3);
        mtv_4 = findViewById(R.id.dir_tv_4);
        mtv_5 = findViewById(R.id.dir_tv_5);

        car_1 = findViewById(R.id.code_car_1);
        /* ------------------------------------------------------------------------------------- */

        /* ------------------------------------------------------------------------------------- */
        // 下拉框效果实现
        String[] ctype = new String[]{"手动模式","光照智能模式","智能语音模式","全自动模式"};
        // 创建一个ArrayAdapter对象
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item,ctype);
        // 为这个适配器设置列表框下拉时的选项样式
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // 将适配器与下拉列表关联起来
        Spinner spinner = findViewById(R.id.spinner);
        spinner.setAdapter(adapter);
        // 获取下拉列表框的选中值
        String choice = spinner.getSelectedItem().toString();
        // 弹窗显示选中值
        //ToastUtil.showmsg(direct_Activity.this,choice);
        /* ------------------------------------------------------------------------------------- */

        // 设置轮播图的页面数量
        car_1.setPageCount(images.length);

        car_1.setImageListener(imageListener);
        // 开启按键点击事件
        mbtn_1.setOnClickListener(this::button_kai);
        mbtn_2.setOnClickListener(this::button_guan);

        // 模式下拉选择框点击事件
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                String result = parent.getItemAtPosition(position).toString();
                ToastUtil.showmsg(direct_Activity.this,result);

                if(result.equals("手动模式")){
                    ToastUtil.showmsg(direct_Activity.this,"手动模式");
                    //publishmessageplus(mqtt_pub_topic,"{\"set_mode\": \"fC}");
                }else if(result.equals("光照智能模式")){
                    ToastUtil.showmsg(direct_Activity.this,"光照智能模式");
                    //publishmessageplus(mqtt_pub_topic,"{\"set_mode\": \"fD}");
                }else if(result.equals("智能语音模式")){
                    ToastUtil.showmsg(direct_Activity.this,"智能语音模式");
                    //publishmessageplus(mqtt_pub_topic,"{\"set_mode\": \"fE}");
                }else if(result.equals("全自动模式")){
                    ToastUtil.showmsg(direct_Activity.this,"全自动模式");
                    //publishmessageplus(mqtt_pub_topic,"{\"set_mode\": \"fF}");
                }

            }
            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        /* -------------------------------------------------------------------------------------- */
        Mqtt_init();
        startReconnect();

        handler = new Handler(Looper.myLooper()) {
            @SuppressLint("SetTextI18n")
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what){
                    case 1: //开机校验更新回传
                        break;
                    case 2:  // 反馈回传

                        break;
                    case 3:  //MQTT 收到消息回传   UTF8Buffer msg=new UTF8Buffer(object.toString());
                        // 如果数据中有"temperature"则提取温度数据并显示
                        if(msg.obj.toString().substring(15, 26).equals("temperature")){
                            String Tem_val = msg.obj.toString().substring(msg.obj.toString().indexOf("temperature\":")+13,msg.obj.toString().indexOf("}"));
                            mtv_2.setText(Tem_val+" ℃");
                        }else if(msg.obj.toString().substring(15, 23).equals("led_flag")){   // 提取led状态数据
                            String led_flg_use =  msg.obj.toString().substring(msg.obj.toString().indexOf("led_flag\":")+10,msg.obj.toString().indexOf("}"));
                            if(led_flg_use.equals("0")){
                                //mtv_3.setText("开");
                                //ToastUtil.showmsg(direct_Activity.this,"ESP8266-LED Open");
                            }else if(led_flg_use.equals("1")){
                                //mtv_3.setText("关");
                                //ToastUtil.showmsg(direct_Activity.this,"ESP8266-LED Close");
                            }else{
                                //mtv_3.setText("关");
                            }

                        }else if(msg.obj.toString().substring(15, 24).equals("wifi_flag")){   // 提取wifi连接状态
                            String wifi_flag =  msg.obj.toString().substring(msg.obj.toString().indexOf("wifi_flag\":")+11,msg.obj.toString().indexOf("}"));
                            if(wifi_flag.equals("1")){
                                // WiFi已连接
                                mtv_4.setText("已连接");
                            }else if(wifi_flag.equals("0")){
                                // WiFi未连接
                                mtv_4.setText("未连接！");
                            }else{
                                mtv_4.setText("等待连接");
                            }

                        }else if(msg.obj.toString().substring(15, 25).equals("Light_flag")){
                            String Lig_flag =  msg.obj.toString().substring(msg.obj.toString().indexOf("Light_flag\":")+12,msg.obj.toString().indexOf("}"));
                            mtv_5.setText(Lig_flag + " Lx");
                            //System.out.println(Lig_flag + " Lx");
                        }
                        System.out.println(msg.obj.toString());

                        break;
                    case 30:  //连接失败
                        //Toast.makeText(CodeActivity.this,"连接失败" ,Toast.LENGTH_SHORT).show();
                        mtv_3.setText("未连接");
                        break;
                    case 31:   //连接成功
                        //Toast.makeText(CodeActivity.this,"连接成功" ,Toast.LENGTH_SHORT).show();
                        mtv_3.setText("已连接");
                        try {
                            client.subscribe(mqtt_sub_topic,1);
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                        break;
                    default:
                        break;
                }
            }
        };
        /* -------------------------------------------------------------------------------------- */


    }




    // 开启控件点击事件
    private void button_kai(View v){
        if(curtain_flag == 1){
            curtain_flag = 0;
            // 发送指令让下位机打开窗帘
            publishmessageplus(mqtt_pub_topic,"{\"set_curtain\": \"fA}");
        }
        mtv_1.setText("开启");
    }
    // 关闭控件点击事件
    private void button_guan(View v){
        if(curtain_flag == 0){
            curtain_flag = 1;
            // 发送指令让下位机关闭窗帘
            publishmessageplus(mqtt_pub_topic,"{\"set_curtain\": \"fB}");
        }
        mtv_1.setText("关闭");
    }

    // 轮播图功能实现
    ImageListener imageListener = new ImageListener() {
        @Override
        public void setImageForPosition(int position, ImageView imageView) {
            imageView.setImageResource(images[position]);
        }
    };


    /* ========================================================================================== */
    // MQTT service function begin to
    // MQTT初始化
    private void Mqtt_init()
    {
        try {
            //host为主机名，test为clientid即连接MQTT的客户端ID，一般以客户端唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存
            client = new MqttClient(host, mqtt_id,
                    new MemoryPersistence());
            //MQTT的连接设置
            MqttConnectOptions options = new MqttConnectOptions();
            //设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录，这里设置为true表示每次连接到服务器都以新的身份连接
            options.setCleanSession(false);
            //设置连接的用户名
            options.setUserName(userName);
            //设置连接的密码
            options.setPassword(passWord.toCharArray());
            // 设置超时时间 单位为秒
            options.setConnectionTimeout(10);
            // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
            options.setKeepAliveInterval(20);
            //设置回调
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    //连接丢失后，一般在这里面进行重连
                    System.out.println("connectionLost----------");
                    //startReconnect();
                }
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    //publish后会执行到这里
                    System.out.println("deliveryComplete---------"
                            + token.isComplete());
                }
                @Override
                public void messageArrived(String topicName, MqttMessage message)
                        throws Exception {
                    //subscribe后得到的消息会执行到这里面
                    System.out.println("messageArrived----------");
                    Message msg = new Message();
                    msg.what = 3;   //收到消息标志位
                    msg.obj = topicName + "---" +message.toString();
                    handler.sendMessage(msg);    // hander 回传
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // MQTT连接函数
    private void Mqtt_connect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    if(!(client.isConnected()) )  //如果还未连接
                    {
                        MqttConnectOptions options = null;
                        client.connect(options);
                        Message msg = new Message();
                        msg.what = 31;
                        handler.sendMessage(msg);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    Message msg = new Message();
                    msg.what = 30;
                    handler.sendMessage(msg);
                }
            }
        }).start();
    }

    // MQTT重新连接函数
    private void startReconnect() {
        scheduler = Executors.newSingleThreadScheduledExecutor();
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                if (!client.isConnected()) {
                    Mqtt_connect();
                }
            }
        }, 0*1000, 10 * 1000, TimeUnit.MILLISECONDS);
    }

    // 订阅函数    (下发任务/命令)
    private void publishmessageplus(String topic,String message2)
    {
        if (client == null || !client.isConnected()) {
            return;
        }
        MqttMessage message = new MqttMessage();
        message.setPayload(message2.getBytes());
        try {
            client.publish(topic,message);
        } catch (MqttException e) {

            e.printStackTrace();
        }
    }
    /* ========================================================================================== */
}