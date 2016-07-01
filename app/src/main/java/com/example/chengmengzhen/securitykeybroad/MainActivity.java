package com.example.chengmengzhen.securitykeybroad;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.example.chengmengzhen.securitykeybroad.view.SercurityDialog;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
    public void click(View view){
        SercurityDialog dialog = new SercurityDialog(this);
        dialog.show();
    }
}
