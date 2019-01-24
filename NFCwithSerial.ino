#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <stdlib.h>

#define START_BYTE 0x7a
#define FINISH_BYTE 0x7b
#define ESCAPE_BYTE 0x7c
#define ESCAPE_MASC 0x20

#define UID01 "5d6b6889"  // 取得した識別子を記述
#define UID02 "5d6b6889"  // 取得した識別子を記述
#define UID03 "5d6b6889"  // 取得した識別子を記述
#define UID04 "5d6b6889"  // 取得した識別子を記述
#define UID05 "5d6b6889"  // 取得した識別子を記述
#define UID05 "5d6b6889"  // 取得した識別子を記述
#define UID06 "5d6b6889"  // 取得した識別子を記述
#define UID07 "5d6b6889"  // 取得した識別子を記述
#define UID08 "5d6b6889"  // 取得した識別子を記述
#define UID09 "5d6b6889"  // 取得した識別子を記述
#define UID10 "5d6b6889"  // 取得した識別子を記述
#define UID11 "5d6b6889"  // 取得した識別子を記述
#define UID12 "5d6b6889"  // 取得した識別子を記述
#define UID13 "5d6b6889"  // 取得した識別子を記述
#define UID14 "5d6b6889"  // 取得した識別子を記述
#define UID15 "5d6b6889"  // 取得した識別子を記述
#define UID16 "5d6b6889"  // 取得した識別子を記述
#define UID17 "5d6b6889"  // 取得した識別子を記述
#define UID18 "5d6b6889"  // 取得した識別子を記述
#define UID19 "5d6b6889"  // 取得した識別子を記述
#define UID20 "5d6b6889"  // 取得した識別子を記述
#define UID21 "5d6b6889"  // 取得した識別子を記述
#define UID22 "5d6b6889"  // 取得した識別子を記述
#define UID23 "5d6b6889"  // 取得した識別子を記述
#define UID24 "5d6b6889"  // 取得した識別子を記述
#define UID25 "5d6b6889"  // 取得した識別子を記述
#define UID26 "5d6b6889"  // 取得した識別子を記述
#define UID27 "5d6b6889"  // 取得した識別子を記述
#define UID28 "5d6b6889"  // 取得した識別子を記述
#define UID29 "5d6b6889"  // 取得した識別子を記述
#define UID30 "5d6b6889"  // 取得した識別子を記述
#define UID31 "5d6b6889"  // 取得した識別子を記述
#define UID32 "5d6b6889"  // 取得した識別子を記述
#define UID33 "5d6b6889"  // 取得した識別子を記述
#define UID34 "5d6b6889"  // 取得した識別子を記述
#define UID35 "5d6b6889"  // 取得した識別子を記述
#define UID36 "5d6b6889"  // 取得した識別子を記述
#define UID37 "5d6b6889"  // 取得した識別子を記述
#define UID38 "5d6b6889"  // 取得した識別子を記述
#define UID39 "5d6b6889"  // 取得した識別子を記述
#define UID40 "5d6b6889"  // 取得した識別子を記述

String Message;
byte NUMBER;
int mode;              // A0可変抵抗器、３モード設定
byte pointDeduct = 1;  // A1ポイント減算
byte pointAdd = 1;     // A2ポイント加算

constexpr uint8_t RST_PIN = 9;     // RSTピンの指定SPI
constexpr uint8_t SS_PIN = 10;     // SSピンの指定SPI
MFRC522 mfrc522(SS_PIN, RST_PIN);  // RC522と接続
MFRC522::MIFARE_Key key;           //認証キーの指定

Adafruit_SSD1306 display(-1);  // ディスプレイ変数の宣言

void setup() {
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // ディスプレイの初期化
    while (!Serial)
        ;
    SPI.begin();
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

    display.clearDisplay();       //一度初期化
    display.setTextSize(2);       //出力する文字の大きさ
    display.setTextColor(WHITE);  //出力する文字の色
    display.setCursor(0, 0);      //カーソル位置の指定
    display.println("Nocard");    // ディスプレイへ表示する文字列
    display.display();            //ディスプレイ情報の更新

    pinMode(3, OUTPUT);         //青
    pinMode(4, OUTPUT);         //赤
    pinMode(A0, INPUT);         //アナログ、モード設定
    pinMode(A1, INPUT_PULLUP);  //ポイント-
    pinMode(A2, INPUT_PULLUP);  //ポイント+
    pinMode(A3, INPUT_PULLUP);  //送信

    Message = "No mode";
    NUMBER = "000";
}

void loop() {
    digitalWrite(3, HIGH);  //初期設定
    digitalWrite(4, LOW);   //初期設定
    noTone(6);              //初期設定
    if (!mfrc522.PICC_IsNewCardPresent())
        return;  // Mifareカードの確認（新しいカードが無ければ終了し、loop関数を繰り返す）
    if (!mfrc522.PICC_ReadCardSerial())
        return;  // Mifareカードのデータ読み込み（読み取れなければ終了し、loop関数を繰り返す）

    String strBuf[mfrc522.uid.size];
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        strBuf[i] = String(mfrc522.uid.uidByte[i], HEX);
        if (strBuf[i].length() == 1) {
            strBuf[i] = "0" + strBuf[i];
        }
    }
    String strUID = strBuf[0] + strBuf[1] + strBuf[2] + strBuf[3];
    Serial.println(strUID);

    if (strUID == UID01 || strUID == UID02 || strUID == UID03 ||
        strUID == UID04 || strUID == UID05 || strUID == UID06 ||
        strUID == UID07 || strUID == UID08 || strUID == UID09 ||
        strUID == UID10 || strUID == UID11 || strUID == UID12 ||
        strUID == UID13 || strUID == UID14 || strUID == UID15 ||
        strUID == UID16 || strUID == UID17 || strUID == UID18 ||
        strUID == UID19 || strUID == UID20 || strUID == UID21 ||
        strUID == UID22 || strUID == UID23 || strUID == UID24 ||
        strUID == UID25 || strUID == UID26 || strUID == UID27 ||
        strUID == UID28 || strUID == UID29 || strUID == UID30 ||
        strUID == UID31 || strUID == UID32 || strUID == UID33 ||
        strUID == UID34 || strUID == UID35 || strUID == UID36 ||
        strUID == UID37 || strUID == UID38 || strUID == UID39 ||
        strUID == UID40) {
        display.clearDisplay();   //一度初期化
        display.setCursor(0, 0);  //一度初期化
        display.println("collect");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("cardUID = ");
        display.println(strUID);
        display.display();

        digitalWrite(3, LOW);
        tone(6, 1700);
        delay(30);
        noTone(6);
        delay(30);
        tone(6, 1700, 90);
        delay(500);

        Select(strUID);  // Option関数へ移行
    }

    else {
        display.clearDisplay();   //一度初期化
        display.setCursor(0, 0);  //一度初期化
        display.println("error!");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("cardUID = ");
        display.println(strUID);
        display.display();

        Serial.println("else");
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        tone(6, 500);
        delay(1000);

        display.clearDisplay();       //一度初期化
        display.setTextSize(2);       // 出力する文字の大きさ
        display.setTextColor(WHITE);  //出力する文字の色
        display.setCursor(0, 0);      //カーソル位置の指定
        display.println("Nocard");  // ディスプレイへ表示する文字列
        display.display();          //ディスプレイ情報の更新
    }
}

void Select(String strUID) {
    byte submit = 1;  // A3送信　INPUT_PULLUPにしているから押さなければHIGH
    byte MODEbyte;
    byte buffer[2];

    while (submit == 0) {
        mode = analogRead(A0);
        pointDeduct = digitalRead(A1);
        pointAdd = digitalRead(A2);
        submit = digitalRead(A3);

        display.clearDisplay();    //一度初期化
        display.setTextSize(2);    // 出力する文字の大きさ
        display.setCursor(0, 0);   //カーソル位置の指定
        display.println(Message);  // ディスプレイへ表示する文字
        display.setTextSize(1);    // 出力する文字の大きさ
        display.print("NUMBER = ");
        display.setTextSize(2);  // 出力する文字の大きさ
        display.println(NUMBER);
        display.display();  //ディスプレイ情報の更新
        delay(300);

        if (mode <= 85) {
            Message = "Reset";
            MODEbyte = 0;
        }

        if (85 < mode && mode < 170) {
            Message = "Point";
            MODEbyte = 1;
        }

        if (170 <= mode) {
            Message = "GameT";
            MODEbyte = 2;
        }

        if (pointDeduct == 0) {
            NUMBER = NUMBER - 10;
        }

        if (pointAdd == 0) {
            NUMBER = NUMBER + 10;
        }

        if (submit == 0) {
            ESPserial(strUID, MODEbyte, NUMBER, buffer);
            break;
        }
    }
}

void ESPserial(String strUID, byte MODEbyte, byte NUMBER, byte* buffer) {
    byte send[6];
    unsigned long UID = strtoul(strUID.c_str(), NULL, 16);
    send[0] = (UID >> 24) & 0xFF;
    send[1] = (UID >> 16) & 0xFF;
    send[2] = (UID >> 8) & 0xFF;
    send[3] = (UID >> 0) & 0xFF;
    send[4] = MODEbyte;
    send[5] = NUMBER;

    byte tmp, checksum = 0;
    unsigned char receive[6];

    SoftwareSerial sfSerial(7, 8);
    sfSerial.begin(9600);
    sfSerial.setTimeout(2500);

    sfSerial.write(START_BYTE);
    for (byte i = 0; i < 6; i++) {
        if (send[i] == START_BYTE || send[i] == ESCAPE_BYTE) {
            sfSerial.write(ESCAPE_BYTE);
            sfSerial.write(send[i] ^ ESCAPE_MASC);
        } else {
            sfSerial.write(send[i]);
        }
    }

    sfSerial.listen();
    while (sfSerial.available() <= 0)
        ;
    if (sfSerial.read() == START_BYTE) {
        for (byte i = 0; i < 6; i++) {
            tmp = sfSerial.read();
            if (tmp == ESCAPE_BYTE) {
                receive[i] = sfSerial.read() ^ ESCAPE_MASC;
            } else {
                receive[i] = tmp;
            }
            checksum += receive[i];
        }
        buffer[0] = receive[0];
        buffer[1] = receive[1];
    }
}
