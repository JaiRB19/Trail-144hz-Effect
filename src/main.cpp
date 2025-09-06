#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

class $modify(MyPlayer, PlayerObject) {
public:
    //variables
    struct Fields {
        int frameSkip = 0;
        int targetSkip = 0;
        int jitterOffset = 0;
    };

    void update(float dt) {
        PlayerObject::update(dt);

        // Inicializar (una sola vez)
        static bool seeded = false;
        if (!seeded) {
            srand(static_cast<unsigned int>(time(nullptr)));
            seeded = true;
        }

        // target/jitter
        if (m_fields->targetSkip == 0) {
            m_fields->targetSkip = 60 + (rand() % 21);  // [60, 80]
            m_fields->jitterOffset = 1 + (rand() % 5);  // [1, 5]
        }

        // jitter incluido
        if (m_fields->frameSkip % (m_fields->targetSkip + m_fields->jitterOffset) == 0) {
            if (m_regularTrail) m_regularTrail->update(dt);
            if (m_shipStreak) m_shipStreak->update(dt);
            if (m_waveTrail) m_waveTrail->update(dt);

            // Nuevo ciclo con valores aleatorios
            m_fields->targetSkip = 60 + (rand() % 21);
            m_fields->jitterOffset = 1 + (rand() % 5);
        }

        m_fields->frameSkip++; //ByJai
    }
};
