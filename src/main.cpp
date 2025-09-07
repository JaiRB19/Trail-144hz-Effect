#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

static bool alwaysActive = false;

$on_mod(Loaded) {
    listenForSettingChanges("always-active", [](bool value) {
        alwaysActive = value;
    });

    // inicializar con valor actual
    alwaysActive = Mod::get()->getSettingValue<bool>("always-active");
}

class $modify(RandomTrailCutter, CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.f;
        float targetTime = 0.f;
        bool isCutting = false;
        bool seeded = false;
        bool wasVisible = false;
    };

    void update(float delta) {
        CCMotionStreak::update(delta);

        auto fields = m_fields.self();

        // inicializar random una sola vez
        if (!fields->seeded) {
            srand(static_cast<unsigned int>(time(nullptr)));
            fields->seeded = true;
            fields->targetTime = 0.3f; 
        }

        fields->wasVisible = this->isVisible();

        if (!fields->wasVisible && !alwaysActive) {
            return;
        }

        // Lógica de corte aleatorio
        fields->elapsedTime += delta;
        if (fields->elapsedTime >= fields->targetTime) {
            fields->elapsedTime = 0.f;

            if (fields->isCutting) {
                this->stopStroke();
            } else {
                this->resumeStroke();
            }

            fields->isCutting = !fields->isCutting;

            // nuevo intervalo aleatorio [0.2s, 0.5s]
            fields->targetTime = 0.2f + (rand() % 4) * 0.1f;
        }

        if (alwaysActive) {
            this->setVisible(true);
        }
        else{
            this->setVisible(false);
        }
    }
};
