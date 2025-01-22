#include <Adafruit_NeoPixel.h>

#define PIN A5        // NeoPixel 데이터 핀
#define NUM_LEDS 12   // NeoPixel LED 개수

Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// HSV to RGB 변환 함수
RGB hsiToRgb(float hue, float intensity) {
  float r, g, b;

  // Hue를 60도로 나누어 섹터 계산
  int i = int(hue / 60) % 6;
  float f = (hue / 60) - i;
  float p = intensity * 0.2;       // 낮은 밝기 기준값 추가
  float q = intensity * (1 - f * 0.8); // 강도 비율 보정
  float t = intensity * (1 - (1 - f) * 0.8); // 상한선 보정

  // 섹터별 RGB 계산
  switch (i) {
    case 0:
      r = intensity, g = t, b = p;
      break;
    case 1:
      r = q, g = intensity, b = p;
      break;
    case 2:
      r = p, g = intensity, b = t;
      break;
    case 3:
      r = p, g = q, b = intensity;
      break;
    case 4:
      r = t, g = p, b = intensity;
      break;
    case 5:
      r = intensity, g = p, b = q;
      break;
  }

  // 결과 값 스케일링
  RGB rgb;
  rgb.r = uint8_t(r * 255);
  rgb.g = uint8_t(g * 255);
  rgb.b = uint8_t(b * 255);

  return rgb;
}

void setup() {
  strip.begin();    // NeoPixel 초기화
  strip.show();     // 모든 LED 끄기
}

void loop() {
  // 가변저항 값 안정화
  int hueValue = smoothAnalogRead(A1);        // Hue (0-1023)
  int intensityValue = smoothAnalogRead(A3); // Intensity (0-1023)

  // Hue와 Intensity를 변환
  float hue = map(hueValue, 0, 1023, 0, 360);        // 0-360으로 매핑
  float intensity = map(intensityValue, 0, 1023, 0, 100) / 100.0; // 0-1로 매핑

  // HSV를 RGB로 변환
  RGB rgb = hsiToRgb(hue, intensity);

  // 모든 LED에 동일한 색상을 설정
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(rgb.r, rgb.g, rgb.b));
  }

  // LED 업데이트
  strip.show();

  // delay(50); // 작은 지연 추가
}

// 아날로그 입력 값을 안정화하는 함수
int smoothAnalogRead(int pin) {
  const int numReadings = 10; // 평균을 위한 샘플 수
  int total = 0;

  for (int i = 0; i < numReadings; i++) {
    total += analogRead(pin);
    delay(5); // 샘플 간의 간격
  }

  return total / numReadings;
}
