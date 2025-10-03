#pragma once

// clang-format off
/* === MODULE MANIFEST V2 ===
module_description: No description provided
constructor_args: []
template_args: []
required_hardware: []
depends: []
=== END MANIFEST === */
// clang-format on

#include "app_framework.hpp"
#include "can.hpp"

class FdcanTx : public LibXR::Application {
public:
  FdcanTx(LibXR::HardwareContainer &hw, LibXR::ApplicationManager &app)
      : can1_(hw.template FindOrExit<LibXR::CAN>({"fdcan1"})),
        can2_(hw.template FindOrExit<LibXR::CAN>({"fdcan2"})),
        can3_(hw.template FindOrExit<LibXR::CAN>({"fdcan3"})) {
    thread_.Create(this, ThreadFunc, "fdcan_tx", 2048,
                   LibXR::Thread::Priority::HIGH);
  }

  void OnMonitor() override {}

  static void ThreadFunc(FdcanTx *fdcan_tx) {
    while (true) {
      fdcan_tx->Fdcan1Send();
      fdcan_tx->Fdcan2Send();
      fdcan_tx->Fdcan3Send();

      for (int i = 0; i < 8; i++) {
        fdcan_tx->data[i]++;
        if (fdcan_tx->data[i] >= 100) {
          fdcan_tx->data[i] = 0;
        }
      }
      LibXR::Thread::Sleep(200);
    }
  }
  void Fdcan1Send() {
    this->now_ = LibXR::Timebase::GetMilliseconds();
    this->dt_ = now_ - this->last_wakeup_;
    this->last_wakeup_ = this->now_;
    can1_tx_pack_.id = 0x01;
    can1_tx_pack_.type = LibXR::CAN::Type::STANDARD;
    memcpy(can1_tx_pack_.data, data, 8);
    can1_->AddMessage(can1_tx_pack_);
  }

  void Fdcan2Send() {
    this->now_ = LibXR::Timebase::GetMilliseconds();
    this->dt_ = now_ - this->last_wakeup_;
    this->last_wakeup_ = this->now_;
    can2_tx_pack_.id = 0x02;
    can2_tx_pack_.type = LibXR::CAN::Type::STANDARD;
    memcpy(can2_tx_pack_.data, data, 8);
    can2_->AddMessage(can2_tx_pack_);
  }

  void Fdcan3Send() {
    this->now_ = LibXR::Timebase::GetMilliseconds();
    this->dt_ = now_ - this->last_wakeup_;
    this->last_wakeup_ = this->now_;
    can3_tx_pack_.id = 0x03;
    can3_tx_pack_.type = LibXR::CAN::Type::STANDARD;
    memcpy(can3_tx_pack_.data, data, 8);
    can3_->AddMessage(can3_tx_pack_);
  }

private:
  float now_;
  float dt_;
  float last_wakeup_;

  LibXR::CAN *can1_;
  LibXR::CAN *can2_;
  LibXR::CAN *can3_;

  LibXR::CAN::ClassicPack can1_tx_pack_{};
  LibXR::CAN::ClassicPack can2_tx_pack_{};
  LibXR::CAN::ClassicPack can3_tx_pack_{};

  LibXR::Thread thread_;

  uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
};
