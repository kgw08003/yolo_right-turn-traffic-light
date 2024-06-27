#include "src/io_Header.h"

const int __attribute__((progmem)) string[][16] = { 
  // 화살표 (16열)
  {0x0000, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x1FFC, 0x1FFC, 0x1FFC, 0x0FF8, 0x07F0, 0x03E0, 0x01C0, 0x0000},
  // 원형 (16열) - 초록색으로 변경
  {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
};

const int __attribute__((progmem)) string2[][16] = { 
  {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, //화살표
  {0x0000, 0x07E0, 0x0FF0, 0x1FF8, 0x3FFC, 0x7FFE, 0x7FFE, 0x7FFE, 0x7FFE, 0x7FFE, 0x7FFE, 0x3FFC, 0x1FF8, 0x0FF0, 0x07E0, 0x0000}, //원형
};

void setup()
{
  DDRB = 0x3f;  // PORTB 5~0 핀 출력으로 설정
  DDRC = 0x08;  // PORTC 3 핀 출력으로 설정
  DDRD = 0xfc;  // PORTD 7~2 핀 출력으로 설정
  Serial.begin(9600); // 시리얼 통신 시작
}

void loop()
{
  static unsigned long lastSwitchTime = 0; // 마지막으로 동작이 전환된 시간을 저장하는 변수
  static int currentPattern = 0; // 현재 사용 중인 패턴 (0 또는 1)
  static int switchInterval = 3000; // 동작을 전환할 시간 간격 (3초)

  unsigned long currentTime = millis(); // 현재 시간을 가져옴

  // 외부에서 신호를 받아서 LED 상태 변경
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '0') {
      currentPattern = 1; // '0'을 받으면 빨간 원 동작
    } else if (receivedChar == '1') {
      currentPattern = 0; // '1'을 받으면 초록색 화살표 동작
    }
  }

  // switchInterval (3초)마다 동작을 전환
  if (currentTime - lastSwitchTime >= switchInterval) {
    lastSwitchTime = currentTime;
    // 아무 동작도 하지 않음
  }

  static unsigned int i_cnt = 0; // for문을 위한 사용자 변수
  unsigned int Buff[16] = { 0 };  // 16x16 도트매트릭스1 문자 사용자 배열
  unsigned int Buff2[16] = { 0 };  // 16x16 도트매트릭스2 출력 사용자 배열

  unsigned char High, Low, High2, Low2 = 0;  // 마스킹을 위한 상위 및 하위 바이트 사용자 변수
  
  // 현재 패턴에 따라 적절한 데이터를 복사
  for (i_cnt = 0; i_cnt < 16; i_cnt++) {
    Buff[i_cnt] = pgm_read_word(&string[currentPattern][i_cnt]);
    Buff2[i_cnt] = pgm_read_word(&string2[currentPattern][i_cnt]);
  }

  for (i_cnt = 0; i_cnt < 16; i_cnt++) {
    High = (Buff[i_cnt] >> 8);  // 16x16 도트매트릭스1 상위 바이트 저장
    Low = (Buff[i_cnt] & 0xff);  // 16x16 도트매트릭스1 하위 바이트 저장
    High2 = (Buff2[i_cnt] >> 8);  // 16x16 도트매트릭스2 상위 바이트 저장
    Low2 = (Buff2[i_cnt] & 0xff);  // 16x16 도트매트릭스2 하위 바이트 저장
    row_dynamic(i_cnt); // 행(Row)을 선택하기 row_dynamic 함수 호출, 인자값으로 i_cnt값이 전달 됨
    if (currentPattern == 0) {
      // 초록색 출력
      Shift_Register(High, Low, i_cnt, Green);  // 저장된 16x16 도트매트릭스1 데이터 전송 (초록색으로 변경)
      Shift_Register(High2, Low2, i_cnt, Green); // 저장된 16x16 도트매트릭스2 데이터 전송 (초록색으로 변경)
    } else {
      // 빨간색 출력
      Shift_Register(High, Low, i_cnt, Red);  // 저장된 16x16 도트매트릭스1 데이터 전송 (빨간색으로 변경)
      Shift_Register(High2, Low2, i_cnt, Red); // 저장된 16x16 도트매트릭스2 데이터 전송 (빨간색으로 변경)
    }
    ActivePulse();  // 데이터 전송 후 래치신호를 LED 드라이버 IC에 전송하는 ActivePulse함수 호출
  }
}
