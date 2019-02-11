#ifndef _CBFTPSERVER_H_
#define _CBFTPSERVER_H_

#include <MenuCallBack.h>
#include "ESP32FtpServer.h"

class CBFTPserver : public MenuCallBack
{
public:
  FtpServer ftpSrv;

  bool setup(){
    ftpSrv.begin("esp32","esp32");    //username, password for ftp.  set ports in ESP32FtpServer.h  (default 21, 50009 for PASV)
    M5.Lcd.fillScreen(0);
    M5.Lcd.setTextColor(0xFFFF, 0);
    M5.Lcd.println("FTP server started...");
    M5.Lcd.println("host : " + WiFi.localIP().toString());
    M5.Lcd.println("user : esp32");
    M5.Lcd.println("pass : esp32");
    M5.Lcd.println();
    M5.Lcd.println("ftp://esp32:esp32@" + WiFi.localIP().toString() + "/");
    return true;
  }

  bool loop()
  {
    ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   

    return true;
  }

private:
};
#endif
