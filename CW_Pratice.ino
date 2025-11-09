#include <EEPROM.h>
#include <Wire.h> 

#include <LiquidCrystal.h>

#define LED 9
#define BUZ 10
#define KEY 16
#define KEYR 17
#define vacantL 16
#define vacantR 17
#define KEY1 11
#define KEY2 12


bool keyState = 0;
bool keyRState = 0;
bool keyState1 = 0;
bool keyState2 = 0;

#define arr_len 49// 字符的数量，用于遍历
byte u_time = 150; // 滴的最长时长，单位毫秒
byte play_u_time = 75; // 滴的最长时长，单位毫秒
#define check_time 20// 防止抖动的忽略时间，少于这个时间的按压会忽略。单位：毫秒
//标点符号可能是6位组合
const char* c[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",  " ", " ",".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", " ", ".--.-.", "..--..", "-..-.", "-.--.-", "-....-", ".-.-.-", "-...-", ".-.-.", "---...", "-.-.-", "..-.."};
const char* w[] = {"A",   "B",    "C",   "D",   "E",  "F",   "G",   "H",    "I",  "J",    "K",   "L",    "M",  "N",  "O",   "P",    "Q",    "R",   "S",   "T", "U",   "V",    "W",   "X",    "Y",    "Z",     " ", " ","1",     "2",     "3",     "4",     "5",     "6",     "7",     "8",     "9",     "0",     " ", "@",      "?",      "/",     "()",     "-",      ".",       "=",     "+",     ":",      ";",     ","};
unsigned long s_time = 0;
unsigned long e_time = 0;// 单次敲击的时间记录
unsigned long cs_time = 0;// 字母，单词间隔时间变量
bool flag_rcd = 0; // 单词敲击的记录开始符号
bool flag_letter = 0;// 字符记录开始符号
bool flag_space = 0;// 检查空格标识
char key_code[30] = "";        // 单次键入的电码
char send_code[100] = "";      // 一次键入的全部电码  
char send_word[100] = "";      // 发送的单词

bool flag_correct = 0;
bool letterFinish = 0;
bool playFinish = 0;
bool playState = 1;
byte mode3Count;
byte randomRange1;
byte randomRange2;
bool eepromOperate = 0;

byte menu = 0;
bool firstSaw = 1;

byte iii;
byte j;
char randomStr1[33] = "";  // 16个字符 + 结束符
char randomStr2[33] = "";

bool displayState1 = 0;
bool displayState2 = 0;

byte randomNum;
bool countState = 0;
bool accessState = 1;
int correctCount = 0;
int errorCount = -1;

byte settingsCount = 0;
byte count1 = 0;
byte count2 = 0;
byte count3;
byte codeShowState = 1;



LiquidCrystal LCD(15, 14, 8,7,6,5);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  EEPROM.begin();
  delay(50);
    
  pinMode(LED, OUTPUT);
  pinMode(BUZ, OUTPUT);
  //pinMode(KEY, INPUT_PULLUP);
  //pinMode(KEYR, INPUT_PULLUP);
  pinMode(vacantL, INPUT_PULLUP);
  pinMode(vacantR, INPUT_PULLUP);
  digitalWrite(LED,0);
  digitalWrite(BUZ,0);
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  randomSeed(analogRead(A0));

  LCD.begin(16, 2); // 初始化LCD，设定显示范围为16列2行
  LCD.print("Hello, World!"); // 显示内容
}
uint16_t tick=0;
char msg[50];
void loop0() {
  // put your main code here, to run repeatedly:
  
  //sprintf(msg,"%06d %d",tick++,digitalRead(KEY2));
  //Serial.println(msg);
  
  LCD.setCursor(0, 1); // 将光标定位到第1列第2行（行列下标从0开始）
  LCD.print(millis() / 10); // 显示运行时间（秒）

  //digitalWrite(LED,tick%2);
  //delay(100);
}


void loop() {
  //sprintf(msg,"%06d KEY2[%d] menu[%d] L[%d]",tick++,digitalRead(KEY2),menu,digitalRead(vacantL));
  //sprintf(msg,"%06d [%s]",tick++,key_code.c_str());
  //Serial.println(msg);
  //strcpy(key_code, "");  // 使用strcpy替代=赋值
 // sprintf(key_code, "%d", tick);  // 数字转字符串
  //sprintf(msg, "%06d [%s] send_code len", tick++, key_code);
  //Serial.println(msg);
 //delay(500);
  //return;
  if (digitalRead(KEY) == LOW) {
    keyState = 1;
  } else {
    keyState = 0;
  }
  if (digitalRead(KEYR) == LOW) {
    keyRState = 1;
  } else {
    keyRState = 0;
  }

  if (digitalRead(KEY1) == LOW)
  { delay(5);
    if (digitalRead(KEY1) == LOW) {
      keyState1 = 1;
      while (digitalRead(KEY1) == LOW);
    }
  } else {
    keyState1 = 0;
  }

  if (digitalRead(KEY2) == LOW)
  { delay(5);
    if (digitalRead(KEY2) == LOW) {
      keyState2 = 1;/*
      EEPROM.write(1, highByte(u_time));
            EEPROM.write(2, lowByte(u_time));
            EEPROM.write(3, highByte(play_u_time));
          EEPROM.write(4, lowByte(play_u_time));
          EEPROM.end();*/
          /*EEPROM.write(1,u_time);
            EEPROM.write(3,play_u_time);
          EEPROM.commit();*/
          //eepromOperate = 1;
      while (digitalRead(KEY2) == LOW);
    }
  } else {
    keyState2 = 0;
  }
  /*if(eepromOperate== 1)
  {
    //EEPROM.write(1,u_time);
    //        EEPROM.write(3,play_u_time);
    EEPROM.write(5, int(codeShowState));
          EEPROM.end();
          eepromOperate = 0;
  }
*/
  if (keyState1 == 1)
  {
    menu += 1;
    settingsCount = 0;
    displayState1 = displayState2 = 0;
    firstSaw = 1;
    randomStr1[0] = randomStr2[0] = 0;
    playFinish = 0;
    playState = 1;
    count3 = 120;
    j = 0;
    mode3Count = 0;
    eepromOperate = 1;
    LCD.clear();
  }

  if (keyState2 == 1)
  {
    eepromOperate = 1;
    LCD.clear();
  }

  check_letter();// 检查字符是否输入完成，停止输入的时间超过一个单位时间的1.2倍则代表此次字符输入完成，将输入的内容进行识别。
  check_key_press();// 检查按键是否按下，按下则开始记录此次输入。
  check_key_release();// 检查按键是否释放

  if (keyState2 == 1) {
    settingsCount ++;
  }
/*
  if(eepromOperate == 1)
          {
            EEPROM.write(1, u_time);
            EEPROM.write(2, play_u_time);
          EEPROM.commit();
            eepromOperate = 0;
          }
*/
  switch (menu) {
    case 0: //常规读取电键输入字符
      if (firstSaw == 1){
        if(keyState1 == 1){
          EEPROM.write(1,u_time);
          EEPROM.write(3,play_u_time);
          ///EEPROM.commit();
        }
        LCD.setCursor(0, 0);
        LCD.print("1  Regular Read");
        LCD.setCursor(0, 1);
        LCD.print("K2 Enter");
        if (keyState2 == 1)
        {
          firstSaw = 0;
          LCD.clear();
        }
      }
      else
      {
        displayState1 = 1;
      }
      break;
    case 1:
      if (firstSaw == 1)
      {
        LCD.setCursor(0, 0);
        LCD.print("2  Random Test");
        LCD.setCursor(0, 1);
        LCD.print("K2 Enter");
        if (keyState2 == 1)
        {
          firstSaw = 0;
          LCD.clear();
        }
      }
      else
      {
        if (accessState == 1)
        {
          if (flag_correct == 1 and randomNum == iii)
          {
            correctCount ++;
          }
          if (flag_correct == 0 or randomNum != iii)
          {
            errorCount ++;
          }
        }
        displayState2 = 1;
        if (accessState == 1)
        {
          randomNum = random(38);
          if(randomNum == 27 or randomNum == 28)
          {
            randomNum = random(38);
          }
          LCD.setCursor(0, 0);
          LCD.print(w[randomNum]);
          LCD.setCursor(2, 0);
          if (codeShowState == 1)
          {
            LCD.print(c[randomNum]);
          }
          else
          {
            LCD.print("      ");
          }
          accessState = 0;
        }
        LCD.setCursor(0, 1);
        LCD.print("right");
        LCD.setCursor(5, 1);
        LCD.print(correctCount);
        LCD.setCursor(8, 1);
        LCD.print("error");
        LCD.setCursor(13, 1);
        LCD.print(errorCount);
      }
      break;
    case 2:
      switch (mode3Count) {
        case 0:
          LCD.setCursor(0, 0);
          LCD.print("3.");
          LCD.setCursor(3, 0);
          LCD.print("Listen & copy");
          LCD.setCursor(0, 1);
          LCD.print("K ENTER");
          LCD.setCursor(2, 0);
          LCD.print(settingsCount + 1);
          LCD.setCursor(8, 1);
          switch (settingsCount) {
            case 0 :
              randomRange1 = 26;
              randomRange2 = 38;
              LCD.print("NUMBERS");
              break;
            case 1:
              LCD.print("LETTERS");
              randomRange1 = 0;
              randomRange2 = 28;
              break;
            case 2:
              LCD.print("COMBINED");
              randomRange1 = 0;
              randomRange2 = 39;
              break;
            default:
              settingsCount = 0;
              break;
          }
          if (keyState == 1)
          {
            mode3Count = 1;
          }
          break;
        case 1:
          if (count3 > 20)
          {
            count3 --;
            LCD.setCursor(0, 0);
            LCD.print("1-Please Ready");
            LCD.setCursor(15, 0);
            LCD.print((count3 - 20)%100);
            LCD.setCursor(0, 1);
            LCD.print("Write Them Down");
          }
          if (count3 == 20)
          {
            LCD.clear();
            count3 = 1;
          }
          if (count3 == 1)
          {
            if (playFinish == 0)
            {
              LCD.setCursor(0, 0);
              LCD.print("2-Playing");
              LCD.setCursor(0, 1);
              LCD.print("Listen Carefully");
            }
            else
            {
              LCD.setCursor(0, 0);
              LCD.print("3-    Done      ");
              LCD.setCursor(0, 1);
              LCD.print("K Check Answer  ");
              if (keyState == 1)
              {
                mode3Count = 2;
              }
            }
            if (playFinish == 0)
            {
              if (letterFinish == 1 or playState == 1)
              {
                playState = 0;
                letterFinish = 0;
                for (int i = 0; i < 16; i ++)
                {
                  randomNum = random(randomRange1, randomRange2);
                  strcat(randomStr1 , w[randomNum]);
                  LCD.setCursor(10, 0);
                  LCD.print(w[randomNum]);
                  LCD.setCursor(11, 0);
                  LCD.print(c[randomNum]);
                  play_code(c[randomNum]);
                  LCD.setCursor(10, 0);
                  LCD.print("      ");
                }
                for (int i = 0; i < 16; i ++)
                {
                  randomNum = random(randomRange1, randomRange2);
                  //randomStr2 += w[randomNum];
                  strcat(randomStr2 , w[randomNum]);
                  LCD.setCursor(10, 0);
                  LCD.print(w[randomNum]);
                  LCD.setCursor(11, 0);
                  LCD.print(c[randomNum]);
                  play_code(c[randomNum]);
                  LCD.setCursor(10, 0);
                  LCD.print("      ");
                }
                playFinish = 1;
              }
            }
          }
          break;
        case 2:
          LCD.setCursor(0, 0);
          LCD.print(randomStr1);
          LCD.setCursor(0, 1);
          LCD.print(randomStr2);
          break;
      }
      break;

    case 3:
      switch (settingsCount) {
        case 0 :
          LCD.setCursor(0, 0);
          LCD.print("4  Settings");
          LCD.setCursor(0, 1);
          LCD.print("K2 Enter");
          break;
        case 1 :
          static int count1 = 0;
          if (keyState == 1) {
            count1 ++;
          }
          if (keyState == 0) {
            count1 = 0;
          }
          if (count1 >= 2)
          {
            u_time ++;
            count1 = 0;
          }
          if (u_time >= 250) {
            LCD.setCursor(10, 1);
            LCD.print("   ");
            u_time = 30;
          }
          LCD.setCursor(0, 0);
          LCD.print("1.Dot Length");
          LCD.setCursor(0, 1);
          LCD.print("K Adjust");
          LCD.setCursor(10, 1);
          LCD.print(u_time);
          LCD.setCursor(14, 1);
          LCD.print("ms");
          
          
                    break;
        case 2 :
          static int count2 = 0;
          if (keyState == 1) {
            count2 ++;
          }
          if (keyState == 0) {
            count2 = 0;
          }
          if (count2 >= 2)
          {
            play_u_time ++;
            count2 = 0;
          }
          if (play_u_time >= 200) {
            LCD.setCursor(10, 1);
            LCD.print("   ");
            play_u_time = 20;
          }
          LCD.setCursor(0, 0);
          LCD.print("2.Play Dot");
          LCD.setCursor(0, 1);
          LCD.print("K Adjust");
          LCD.setCursor(10, 1);
          LCD.print(play_u_time);
          LCD.setCursor(14, 1);
          LCD.print("ms");
          if(keyState2 == 1)
          {
            EEPROM.write(1,u_time);
            EEPROM.write(3,play_u_time);
          //EEPROM.commit();
          }
          break;
        case 3:
          if (digitalRead(KEY) == LOW or digitalRead(vacantL) == LOW)
          { delay(5);
            if (digitalRead(KEY) == LOW) {
              codeShowState += 1;
              EEPROM.write(5, codeShowState);
          //EEPROM.commit();
              while (digitalRead(KEY) == LOW);
            }
          }
          LCD.setCursor(11, 1);
          if(codeShowState > 1)
          {
            codeShowState = 0;
          }
          if (codeShowState == 1)
          {
            LCD.print("Show");
          }
          if (codeShowState == 0)
          {
            LCD.print("Hide");
          }
          LCD.setCursor(0, 0);
          LCD.print("3.Morse Code");
          LCD.setCursor(0, 1);
          LCD.print("K Adjust");
          if(keyState2 == 1)
          {
            EEPROM.write(1,u_time);
            EEPROM.write(3,play_u_time);
          //EEPROM.commit();
          }
          break;
        case 4:
          LCD.setCursor(0, 0);
          LCD.print("PCB  Ver 2.0-1");
          LCD.setCursor(0, 1);
          LCD.print("Code Ver 1.0.15");
          break;
        case 5:
          LCD.setCursor(0, 0);
          LCD.print("WeChat");
          LCD.setCursor(0, 1);
          LCD.print("QQ2652283064");
          break;
        default:
          settingsCount = 1;
          break;
      }
      break;
    default :
      menu = 0;
      break;
  }
}
// 修改play_code函数参数类型
void play_code(const char* p_code) {
  for (j = 0; j < strlen(p_code); j++) {  // 使用strlen获取长度
    if (p_code[j] == '.') {
      digitalWrite(LED, 1);
      tone(BUZ, 370);
      delay(play_u_time * 1.1);
      digitalWrite(LED, 0);
      noTone(BUZ);
      delay(play_u_time * 1.05);
    }
    else if (p_code[j] == '-') {
      digitalWrite(LED, 1);
      tone(BUZ, 370);
      delay(play_u_time * 3.15);
      digitalWrite(LED, 0);
      noTone(BUZ);
      delay(play_u_time * 1.05);
    }
    else if (p_code[j] == ' ') {
      delay(play_u_time * 5.25);
    }
    if ((j + 1) == strlen(p_code)) {
      letterFinish = 1;
      delay(play_u_time);
    }
  }
  delay(play_u_time * 1.5);
}

void check_letter() {
  if (flag_letter == 1) {
    int diff_letter = millis() - cs_time;
    if (diff_letter > u_time * 1 && strlen(key_code) > 0) {  // 使用strlen检查是否为空
      check_code(key_code);
      
      // 使用strcat和strcpy替代String拼接
      strcat(send_code, key_code);
      strcat(send_code, " ");
      
      strcpy(key_code, "");  // 清空key_code
      flag_letter = 0;
      accessState = 1;
      flag_space = 1;
      cs_time = millis();
    }
  }

  if (flag_space == 1) {
    int diff_space = millis() - cs_time;
    if (diff_space > u_time * 3) {
      // 移除最后一个空格
      if (strlen(send_code) > 0) {
        send_code[strlen(send_code)-1] = '\0';
      }
      flag_space = 0;
      cs_time = millis();
    }
  }
}

void check_key_press() {
  if (digitalRead(KEY) == 0 or digitalRead(vacantL) == LOW) {
    if (flag_rcd == 0) {
      flag_letter = 0;
      flag_space = 0;
      flag_rcd = 1;
      s_time = millis();
      if (displayState1 == 1 or displayState2 == 1) {
        digitalWrite(LED, 1);
        tone(BUZ, 370);
      }
    }
  }
}

void check_key_release() {
  if (digitalRead(KEY) == 1 and digitalRead(vacantL) == 1) {
    if (flag_rcd == 1) {
      digitalWrite(LED, 0);
      noTone(BUZ);
      e_time = millis();
      unsigned long diff_time = e_time - s_time;

      if (diff_time > check_time) {
        // 使用strcat替代String的+=
        if (diff_time < u_time) {
          strcat(key_code, ".");
        }
        else if (diff_time >= u_time) {
          strcat(key_code, "-");
        }
        flag_rcd = 0;
        //sprintf(msg, "%06d [%s] %lu", tick++, key_code, diff_time);
        //Serial.println(msg);
        cs_time = millis();
        flag_letter = 1;
      }
      else {
        flag_rcd = 0;
        flag_letter = 1;
      }
    }
  }
}

// 修改参数类型为const char*
bool check_code(const char* code) {
  for (int i = 0; i < arr_len; i++) {
    if (strcmp(code, c[i]) == 0) {  // 使用strcmp比较字符串
      LCD.clear();
      if (displayState1 == 1) {
        LCD.setCursor(0, 0);
        LCD.print(w[i]);
        LCD.setCursor(2, 0);
        LCD.print(key_code);
      }
      if (displayState2 == 1) {
        LCD.setCursor(8, 0);
        LCD.print(w[i]);
        LCD.setCursor(10, 0);
        LCD.print(key_code);
      }
      iii = i;
      flag_correct = 1;
      
      if (displayState1 == 1) {
        if (flag_correct == 1) {
          // 使用strcat拼接字符串
          strcat(send_word, w[iii]);
        }
        else {
          strcat(send_word, " ");
        }
        if (strlen(send_word) >= 16) {
          strcpy(send_word, "");
        }
        LCD.setCursor(0, 1);
        LCD.print(send_word);
      }
      return true;
    }
  }
  
  if (displayState1 == 1 or displayState2 == 1) {
    LCD.clear();
    LCD.setCursor(9, 0);
    LCD.print("error");
  }
  flag_correct = 0;
  return false;
}

// 需要修改的其他函数（在您的switch case中）
// 在mode3Count == 1的情况下，需要修改randomStr1和randomStr2的拼接
void updateRandomStrings() {
  // 原来的：randomStr1 += w[randomNum];
  // 改为：
  strcat(randomStr1, w[randomNum]);
  strcat(randomStr2, w[randomNum]);
}
