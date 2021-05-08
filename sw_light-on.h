#pragma once

class GpioPi {
 public:
  GpioPi();
  ~GpioPi();
  void SetCallback(); // GPIOの入力値変化通知の設定

 private:
  void TackSwitchPinInit(); // タクトスイッチPinの初期化
//  void GpioEventNotify(int, unsigned int, unsigned int, unsigned int); // コールバック関数

  int32_t pi_; // pigpioをstart結果

  std::function<void(uint8_t)> tactsw_notify_;
  // ↑今後監視するポートが増えたら配列にする
  //std::array<std::function, kTactSwPin> notify_func_list;

  static constexpr auto kSamplingTime = 200*1000; // チャタキャンセルのサンプリング数(us)

};

