package com.example.chengmengzhen.securitykeybroad;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.example.chengmengzhen.securitykeybroad.jninative.JniEncode;
import com.example.chengmengzhen.securitykeybroad.utils.DESCoding;
import com.example.chengmengzhen.securitykeybroad.view.SercurityDialog;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void click(View view) {
        SercurityDialog dialog = new SercurityDialog(this);
        dialog.setOnInputCompleteListener(new SercurityDialog.InputCompleteListener() {
            @Override
            public void inputComplete(String PassWord) {
                //把password转为byte数组
                String[] pwdStrings = PassWord.replace("[", "").replace("]", "").split(",");
                byte[] PwdBytes = new byte[pwdStrings.length];
                for (int i = 0; i < PwdBytes.length; i++) {
                    PwdBytes[i] = Byte.valueOf(pwdStrings[i]);
                }
                //jni加密
                String enPwd = JniEncode.encryption(PwdBytes);

                //加密后的byte数组
                String[] split = enPwd.replace("[", "").replace("]", "").split(",");
                byte[] enPwdBytes = new byte[split.length];
                for (int i = 0; i < enPwdBytes.length; i++) {
                    enPwdBytes[i] = Byte.valueOf(split[i].trim());
                }

                byte[] key = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

                //解密
                final byte[] deSedes = new DESCoding().decode(enPwdBytes, key, "DESede");
                Toast.makeText(MainActivity.this, "你输入的密码是：" + Arrays.toString(deSedes), Toast.LENGTH_LONG).show();
            }
        });
        dialog.show();
    }
}
