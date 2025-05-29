#pragma once

class Assets
{
private:
    Assets() {};

public:
    // Player
    const char *idle = "resources/assets/character/Idle_Right.png";
    const char *hit_1 = "resources/assets/character/Hit_Right_1.png";
    const char *hit_2 = "resources/assets/character/Hit_Right_2.png";
    const char *hit_3 = "resources/assets/character/Hit_Right_3.png";
    const char *hit_4 = "resources/assets/character/Hit_Right_4.png";
    const char *hit_5 = "resources/assets/character/Hit_Right_5.png";
    const char *hit_loong = "resources/assets/character/Hit_Long(looping).png";

    // Background
    const char *background = "resources/Assets/BackGround/BG.png";
    const char *crowd = "resources/Assets/Sprite Sheet Crowd/Crowd.png";
    const char *stage = "resources/Assets/Stage/Stage.png";

    // Note
    const char *tap_note_left = "resources/Assets/Notes/Notes Left.png";
    const char *tap_note_right = "resources/Assets/Notes/Notes Right.png";
    const char *hold_note_left = "resources/Assets/Notes/Long Notes Left p.png";
    const char *hold_note_right = "resources/Assets/Notes/Long Notes Right p.png";

    // Music
    const char *music = "resources/Assets/music/audio.mp3";

    // SFX
    const char *hit_sfxs[4] = {
        "resources/Assets/music/normal-hitclap.ogg",
        "resources/Assets/music/normal-hitclap2.ogg",
        "resources/Assets/music/normal-hitfinish.ogg",
        "resources/Assets/music/normal-hitnormal.ogg",
    };

    const char *hold_sfx = "resources/Assets/music/spinnerbonus.ogg";

    static Assets &GetInstance()
    {
        static Assets instance;
        return instance;
    }
    Assets(Assets const &) = delete;
    void operator=(Assets const &) = delete;
};
