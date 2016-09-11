package com.example.chengmengzhen.securitykeybroad.view;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.chengmengzhen.securitykeybroad.BaseApplication;
import com.example.chengmengzhen.securitykeybroad.R;

import java.util.ArrayList;

/**
 * Created by chengmengzhen on 16/6/30.
 */
public class SercurityDialog extends Dialog implements View.OnClickListener {
    /**
     * 0-9,数字按钮
     */
    private Button mNum1;
    private Button mNum2;
    private Button mNum3;
    private Button mNum4;
    private Button mNum5;
    private Button mNum6;
    private Button mNum7;
    private Button mNum8;
    private Button mNum9;
    private Button mNum0;
    private LinearLayout mDelPwd;

    private ArrayList<ImageView> mPwdImgs = new ArrayList<ImageView>();
    private ImageView mPwdImg1;
    private ImageView mPwdImg2;
    private ImageView mPwdImg3;
    private ImageView mPwdImg4;
    private ImageView mPwdImg5;
    private ImageView mPwdImg6;

    private TextView mTv;

    private int mPwdCountNum;
    private InputCompleteListener mInputCompleteListener;

    public interface InputCompleteListener {
        public void inputComplete(String passWord);
    }

    public void setOnInputCompleteListener(InputCompleteListener inputCompleteListener) {
        this.mInputCompleteListener = inputCompleteListener;
    }

    public SercurityDialog(Context context) {
        super(context, R.style.SercurityDialogTheme);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //给dialog设置布局
        setContentView(R.layout.ui_security_dialog);
        //通过window设置获取dialog参数
        Window window = getWindow();
        WindowManager.LayoutParams params = window.getAttributes();

        //获取屏幕的宽高
        WindowManager manager = (WindowManager) getContext()
                .getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics outMetrics = new DisplayMetrics();
        manager.getDefaultDisplay().getMetrics(outMetrics);
        int width = outMetrics.widthPixels;
        int height = outMetrics.heightPixels;

        //设置dialog的宽
        params.width = width;
        //设置dialog在屏幕中的位置
        params.gravity = Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL;
        //设置dialog属性
        window.setAttributes(params);

        initView();
        initListener();
    }

    private void initListener() {
        mNum0.setOnClickListener(this);
        mNum1.setOnClickListener(this);
        mNum2.setOnClickListener(this);
        mNum3.setOnClickListener(this);
        mNum4.setOnClickListener(this);
        mNum5.setOnClickListener(this);
        mNum6.setOnClickListener(this);
        mNum7.setOnClickListener(this);
        mNum8.setOnClickListener(this);
        mNum9.setOnClickListener(this);
        mDelPwd.setOnClickListener(this);
    }


    private void initView() {
        mNum0 = (Button) findViewById(R.id.button0);
        mNum1 = (Button) findViewById(R.id.button1);
        mNum2 = (Button) findViewById(R.id.button2);
        mNum3 = (Button) findViewById(R.id.button3);
        mNum4 = (Button) findViewById(R.id.button4);
        mNum5 = (Button) findViewById(R.id.button5);
        mNum6 = (Button) findViewById(R.id.button6);
        mNum7 = (Button) findViewById(R.id.button7);
        mNum8 = (Button) findViewById(R.id.button8);
        mNum9 = (Button) findViewById(R.id.button9);
        mDelPwd = (LinearLayout) findViewById(R.id.button_del);

        mPwdImg1 = (ImageView) findViewById(R.id.pwd_1);
        mPwdImg2 = (ImageView) findViewById(R.id.pwd_2);
        mPwdImg3 = (ImageView) findViewById(R.id.pwd_3);
        mPwdImg4 = (ImageView) findViewById(R.id.pwd_4);
        mPwdImg5 = (ImageView) findViewById(R.id.pwd_5);
        mPwdImg6 = (ImageView) findViewById(R.id.pwd_6);
        mPwdImgs.add(mPwdImg1);
        mPwdImgs.add(mPwdImg2);
        mPwdImgs.add(mPwdImg3);
        mPwdImgs.add(mPwdImg4);
        mPwdImgs.add(mPwdImg5);
        mPwdImgs.add(mPwdImg6);

    }

    private String mPassWord = "";

    @Override
    public void onClick(View view) {

        //删除 如果输入密码个数是0 return ，要不就mPwdCountNum 减1
        if (view.getId() == R.id.button_del) {
            if (mPwdCountNum == 0) {
                return;
            } else {
                mPwdCountNum = mPwdCountNum - 1;

                if (mPwdCountNum == 0){
                    mPassWord = mPassWord.substring(0,0);
                }else {
                    // 这里-2的原因是因为数字之间有一个逗号，逗号+数字=2
                    mPassWord = mPassWord.substring(0,mPassWord.length()-2);
                }

                showPwdImages(mPwdCountNum);
            }
        }
        //
        else {
            mPwdCountNum++;
            showPwdImages(mPwdCountNum);
            inputPwd(view);
        }

        /**
         * 当统计的当前输入的密码位数大于等于6，表示输入完成，数据处理在UI线程中处理
         */
        if (mPwdCountNum >= 6) {
            /**
             * 开启一个线程，将密码数字传送给UI线程去加密
             */
            new Thread() {
                @Override
                public void run() {
                    SystemClock.sleep(100);
                    dismiss();              // 关闭对话框

                    // 让BaseApplication中的Handler来处理密码加密
                    if (mInputCompleteListener != null) {
                        BaseApplication.getHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                mInputCompleteListener.inputComplete(mPassWord);
                            }
                        });
                    }
                }
            }.start();

        }
    }

    /**
     * 通过获取Button上的数字字符来合成字符串
     *
     * @param view
     */
    private void inputPwd(View view) {
        if (mPwdCountNum > 1) {
            mPassWord = mPassWord + ",";
        }

        mPassWord += ((Button)view).getText();
    }

    /**
     * 根据传入的参数来设置显示的密码的图片张数
     *
     * @param pwdCountNum
     */
    private void showPwdImages(int pwdCountNum) {

        for(int i = 0; i < mPwdImgs.size(); i++) {
            if(i < pwdCountNum) {
                mPwdImgs.get(i).setVisibility(View.VISIBLE);
            } else {
                mPwdImgs.get(i).setVisibility(View.GONE);
            }
        }
    }

}
