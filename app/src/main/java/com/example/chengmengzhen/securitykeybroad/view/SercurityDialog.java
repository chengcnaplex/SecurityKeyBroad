package com.example.chengmengzhen.securitykeybroad.view;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.example.chengmengzhen.securitykeybroad.R;

/**
 * Created by chengmengzhen on 16/6/30.
 */
public class SercurityDialog extends Dialog{
    private View mRootView;
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
        params.gravity = Gravity.BOTTOM|Gravity.CENTER_HORIZONTAL;
        //设置dialog属性
        window.setAttributes(params);

        initView();
    }


    private void initView() {
//        findViewById()findViewById
    }
}
