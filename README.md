# SecurityKeyBroad

##效果图
![image](https://github.com/chengcnaplex/SecurityKeyBroad/blob/master/gifres/SecurityKeyBroad.gif)

##使用
    
        SercurityDialog dialog = new SercurityDialog(this);
        dialog.setOnInputCompleteListener(new SercurityDialog.InputCompleteListener() {
            @Override
            public void inputComplete(String PassWord) {
                //加密
                //作者这里使用jni加密，需要了解的可以查看源码
            }
        });
        dialog.show();
    
