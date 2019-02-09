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
    M5.Lcd.println("FTP server start...");
    M5.Lcd.println("user : esp32");
    M5.Lcd.println("pass : esp32");
    return true;
  }

  bool loop()
  {
    M5.update();
    ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   

    return !M5.BtnA.wasReleased();
  }

private:
};
#endif
