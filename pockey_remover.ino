#include <Servo.h>

// サーボオブジェクトの作成
Servo servo_slider;  // MG996R
Servo servo_peeler;  // MG996R
Servo servo_pockey;  // MG90S

// ピン番号の定義
const int PIN_PEELER = 7;
const int PIN_SLIDER = 8;
const int PIN_POCKEY = 9;

// 初期角度
const int INITIAL_ANGLE = 60;

// 各モジュール角度
const int SLIDER_ANGLE_1 = 90;
const int SLIDER_ANGLE_2 = 10;
const int PEELER_ANGLE_1 = 120;
const int PEELER_ANGLE_2 = 90;
const int POCKEY_ANGLE_1 = 0;
const int POCKEY_ANGLE_2 = 90;
const int POCKEY_ANGLE_3 = 180;

// 削って良いかのフラグ
bool canRemove = false;

// 現在のステップ
int currentStep = 0;

void setup() {
  // シリアル通信の開始
  Serial.begin(9600);

  // サーボをピンにアタッチ
  servo_slider.attach(PIN_SLIDER);
  servo_peeler.attach(PIN_PEELER);
  servo_pockey.attach(PIN_POCKEY);

  servo_slider.write(SLIDER_ANGLE_1);
  servo_peeler.write(PEELER_ANGLE_1);
  servo_pockey.write(POCKEY_ANGLE_1);

  Serial.println("=== サーボ制御システム起動 ===");
  Serial.println("コマンド: 'a' を送信すると次のステップに進みます");
  Serial.println("ステップ0で待機中...");
  Serial.println();
}

void loop() {
  // シリアルモニタからの入力をチェック
  if (Serial.available() > 0) {
    char input = Serial.read();

    // aが送られてきたら自動で動作開始
    if (input == 'a' || input == 'A') {
      canRemove = true;
    }
  }

  if (canRemove == false) {
    return;
  }

  if (currentStep % 2 == 0) {
    Serial.println("--- ポッキーのチョコレートを削る処理 ---");
    removeChocolate();
  } else {
    Serial.println("--- ポッキーを回す処理 ---");
    rotatePockey(currentStep);
  }
  currentStep++;

  // ステップ19の後は1に戻る
  if (currentStep > 19) {
    currentStep = 0;
    // 動作をストップ
    canRemove = false;
    Serial.println();
    Serial.println("--- 1サイクル完了。次の 'a' でステップ1から再開します ---");
    Serial.println();
  }
}

void removeChocolate() {
  servo_peeler.write(PEELER_ANGLE_2);
  Serial.println("servo_peeler = " + String(PEELER_ANGLE_2) + "度");
  delay(1000);
  servo_slider.write(SLIDER_ANGLE_2);
  Serial.println("servo_slider = " + String(SLIDER_ANGLE_2) + "度");
  delay(1000);
  servo_peeler.write(PEELER_ANGLE_1);
  Serial.println("servo_peeler = " + String(PEELER_ANGLE_1) + "度");
  delay(1000);
  servo_slider.write(SLIDER_ANGLE_1);
  Serial.println("servo_slider = " + String(SLIDER_ANGLE_1) + "度");
  delay(2000);
}

void rotatePockey(int step) {
  int deg = 20 * (1 + int(step / 2));
  if (deg > 180) {
    deg = 0;
  }
  servo_pockey.write(deg);
  Serial.println("servo_pockey = " + String(deg) + "度");
}