// character_snapshot.h
#ifndef CHARACTER_SNAPSHOT_H_
#define CHARACTER_SNAPSHOT_H_

#include "bullet_packet.h"
#include <nan2/math/vector2.h>

#include <iostream>
#include <vector>

namespace nan2 {

  class CharacterSnapshot {

  private:

    int snapshot_time_;
    std::vector<BulletPacket> bullet_packets_;
    Vector2 position_;
    int dash_time_;
    int dash_cooldown_;
    unsigned char dash_dir_;
    int weapon_magazine_;
    int weapon_ammo_;
    int weapon_cooldown_;
    int weapon_reload_time_;

  public:

    CharacterSnapshot();

    void Update(int dt);
    void AddBulletPacket(BulletPacket& bullet);
    void SetPosition(const Vector2& position);
    void SetDashTime(int dash_time);
    void SetDashCooldown(int dash_cooldown);
    void SetDashDir(unsigned char dash_dir);
    void SetWeaponMagazine(int magazine);
    void SetWeaponAmmo(int ammo);
    void SetWeaponCooldown(int cooldown);
    void SetWeaponReloadTime(int reload_time);

    int snapshot_time() const;
    const std::vector<BulletPacket>& bullet_packets() const;
    const Vector2& position() const;
    int dash_time() const;
    int dash_cooldown() const;
    unsigned char dash_dir() const;
    int weapon_magazine() const;
    int weapon_ammo() const;
    int weapon_cooldown() const;
    int weapon_reload_time() const;

    void Clear();

    friend std::ostream& operator<<(std::ostream& os, const CharacterSnapshot& snapshot);

  };
  
}

#endif