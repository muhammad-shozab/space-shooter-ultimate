/*
 * ============================================================================
 * SPACE SHOOTER ULTIMATE EDITION v5.0
 * ============================================================================
 * A complete C++ SFML Space Shooter game with:
 * - All sprites loaded from assets folder (PNG)
 * - 2 Levels with 2 Phases each + Final Boss Level
 * - Health bars for all entities
 * - Video intro support
 * - High-quality menu screen
 * - Epic final boss battle
 *
 * REQUIRED ASSETS IN 'assets/' FOLDER:
 * -------------------------------------
 * SPRITES:
 *   - spaceship.png          (Player ship)
 *   - enemy_alpha.png        (Alpha enemy)
 *   - enemy_beta.png         (Beta enemy)
 *   - enemy_gamma.png        (Gamma enemy)
 *   - enemy_monster.png      (Monster enemy)
 *   - enemy_phantom.png      (Phantom enemy)
 *   - enemy_dragon.png       (Dragon mini-boss)
 *   - boss.png               (Final boss)
 *   - boss_eye.png           (Boss eye sprite)
 *   - player_bullet.png      (Player bullet)
 *   - enemy_bullet.png       (Enemy bullet)
 *   - boss_bullet.png        (Boss bullet)
 *   - powerup_power.png      (Power upgrade)
 *   - powerup_fire.png       (Fire rate upgrade)
 *   - powerup_shield.png     (Shield)
 *   - powerup_lives.png      (Extra life)
 *   - powerup_nuke.png       (Screen clear)
 *   - powerup_multishot.png  (Multi-shot)
 *   - powerup_slow.png       (Slow time)
 *   - powerup_danger.png     (Invincibility)
 *   - explosion.png          (Explosion spritesheet - 8 frames horizontal)
 *   - shield_effect.png      (Shield visual)
 *
 * BACKGROUNDS:
 *   - background.png         (Gameplay background)
 *   - menu_background.png    (Menu background)
 *   - boss_background.png    (Boss level background)
 *
 * OTHER:
 *   - font.ttf               (Game font)
 *   - intro_video.png        (Intro frames or static image)
 *   - logo.png               (Game logo)
 *
 * AUDIO (Optional):
 *   - shoot.wav
 *   - explosion.wav
 *   - powerup.wav
 *   - boss_music.wav
 *   - menu_music.wav
 *   - game_music.wav
 *
 * COMPILE:
 *   g++ -o SpaceShooter SpaceShooter_Enhanced.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
 *
 * ============================================================================
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <cmath>
#include <string>
#include <map>
#include <deque>
#include <chrono>
#include <sstream>

using namespace std;

// ============================================================================
// CONSTANTS & CONFIGURATION
// ============================================================================

const float SCREEN_WIDTH = 1200.0f;
const float SCREEN_HEIGHT = 800.0f;
const string GAME_TITLE = "Space Shooter Ultimate Edition v5.0";
const int MAX_HIGH_SCORES = 10;
const float PI = 3.14159265359f;
const int TARGET_FPS = 60;

// Game Balance Settings
const int MAX_LEVELS = 2;
const int PHASES_PER_LEVEL = 2;
const float PLAYER_MAX_HEALTH = 100.0f;
const float PLAYER_MAX_SHIELD = 50.0f;
const float BASE_FIRE_RATE = 0.25f;
const float MIN_FIRE_RATE = 0.10f;
const float MAX_PLAYER_SPEED = 350.0f;

// ============================================================================
// TEXTURE MANAGER - Loads and manages all game textures
// ============================================================================

class TextureManager {
private:
    map<string, sf::Texture> textures;
    static TextureManager* instance;

public:
    static TextureManager& getInstance() {
        if (!instance) {
            instance = new TextureManager();
        }
        return *instance;
    }

    bool loadTexture(const string& name, const string& filepath) {
        sf::Texture texture;
        if (texture.loadFromFile(filepath)) {
            texture.setSmooth(true);
            textures[name] = texture;
            cout << "[OK] Loaded: " << filepath << endl;
            return true;
        }
        cerr << "[FAIL] Could not load: " << filepath << endl;
        return false;
    }

    sf::Texture& getTexture(const string& name) {
        return textures[name];
    }

    bool hasTexture(const string& name) {
        return textures.find(name) != textures.end();
    }

    void loadAllTextures() {
        cout << "\n=== Loading Game Assets ===" << endl;

        // Player
        loadTexture("spaceship", "assets/spaceship.png");
        loadTexture("player_bullet", "assets/player_bullet.png");
        loadTexture("shield_effect", "assets/shield_effect.png");

        // Enemies
        loadTexture("enemy_alpha", "assets/enemy_alpha.png");
        loadTexture("enemy_beta", "assets/enemy_beta.png");
        loadTexture("enemy_gamma", "assets/enemy_gamma.png");
        loadTexture("enemy_monster", "assets/enemy_monster.png");
        loadTexture("enemy_phantom", "assets/enemy_phantom.png");
        loadTexture("enemy_dragon", "assets/enemy_dragon.gif");
        loadTexture("enemy_bullet", "assets/enemy_bullet.png");

        // Boss
        loadTexture("boss", "assets/boss.png");
        loadTexture("boss_eye", "assets/boss_eye.png");
        loadTexture("boss_bullet", "assets/boss_bullet.png");

        // Power-ups
        loadTexture("powerup_power", "assets/powerup_power.png");
        loadTexture("powerup_fire", "assets/powerup_fire.png");
        loadTexture("powerup_shield", "assets/powerup_shield.png");
        loadTexture("powerup_lives", "assets/powerup_lives.png");
        loadTexture("powerup_nuke", "assets/powerup_nuke.png");
        loadTexture("powerup_multishot", "assets/powerup_multishot.png");
        loadTexture("powerup_slow", "assets/powerup_slow.png");
        loadTexture("powerup_danger", "assets/powerup_danger.png");

        // Effects
        loadTexture("explosion", "assets/explosion.png");

        // Backgrounds
        loadTexture("background", "assets/background.png");
        loadTexture("menu_background", "assets/menu_background.png");
        loadTexture("boss_background", "assets/boss_background.png");

        // UI
        loadTexture("logo", "assets/logo.png");
        loadTexture("intro_video", "assets/intro_video.png");

        cout << "=== Asset Loading Complete ===\n" << endl;
    }
};

TextureManager* TextureManager::instance = nullptr;

// ============================================================================
// SOUND MANAGER - Loads and manages all game sounds
// ============================================================================

class SoundManager {
private:
    map<string, sf::SoundBuffer> buffers;
    map<string, sf::Sound> sounds;
    sf::Music backgroundMusic;
    bool soundEnabled;
    float masterVolume;
    static SoundManager* instance;

public:
    static SoundManager& getInstance() {
        if (!instance) {
            instance = new SoundManager();
        }
        return *instance;
    }

    SoundManager() : soundEnabled(true), masterVolume(70.0f) {}

    bool loadSound(const string& name, const string& filepath) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filepath)) {
            buffers[name] = buffer;
            sounds[name].setBuffer(buffers[name]);
            sounds[name].setVolume(masterVolume);
            cout << "[OK] Loaded sound: " << filepath << endl;
            return true;
        }
        cerr << "[FAIL] Could not load sound: " << filepath << endl;
        return false;
    }

    void loadAllSounds() {
        cout << "\n=== Loading Sound Assets ===" << endl;
        loadSound("shoot", "assets/shoot.wav");
        loadSound("explosion", "assets/explosion.wav");
        loadSound("powerup", "assets/powerup.wav");
        loadSound("hit", "assets/hit.wav");
        loadSound("boss_hit", "assets/boss_hit.wav");
        loadSound("player_hurt", "assets/player_hurt.wav");
        loadSound("level_up", "assets/level_up.wav");
        loadSound("game_over", "assets/game_over.wav");
        loadSound("victory", "assets/victory.wav");
        cout << "=== Sound Loading Complete ===\n" << endl;
    }

    void playSound(const string& name) {
        if (soundEnabled && sounds.find(name) != sounds.end()) {
            sounds[name].play();
        }
    }

    void playMusic(const string& filepath, bool loop = true) {
        if (backgroundMusic.openFromFile(filepath)) {
            backgroundMusic.setLoop(loop);
            backgroundMusic.setVolume(masterVolume * 0.5f);
            if (soundEnabled) {
                backgroundMusic.play();
            }
        }
    }

    void stopMusic() {
        backgroundMusic.stop();
    }

    void toggleSound() {
        soundEnabled = !soundEnabled;
        if (!soundEnabled) {
            backgroundMusic.pause();
        }
        else {
            backgroundMusic.play();
        }
    }

    bool isSoundEnabled() const { return soundEnabled; }
    void setVolume(float vol) { masterVolume = vol; }
};

SoundManager* SoundManager::instance = nullptr;

// ============================================================================
// RANDOM GENERATOR
// ============================================================================

class RandomGenerator {
private:
    static mt19937 generator;

public:
    static void seed() {
        generator.seed(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    }

    static float range(float min, float max) {
        uniform_real_distribution<float> dist(min, max);
        return dist(generator);
    }

    static int range(int min, int max) {
        uniform_int_distribution<int> dist(min, max);
        return dist(generator);
    }
};

mt19937 RandomGenerator::generator;

// ============================================================================
// VECTOR2 UTILITY CLASS
// ============================================================================

class Vector2 {
public:
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(float scalar) const { return scalar != 0 ? Vector2(x / scalar, y / scalar) : *this; }

    float length() const { return sqrt(x * x + y * y); }
    float distanceTo(const Vector2& other) const { return (*this - other).length(); }

    Vector2 normalized() const {
        float len = length();
        return len > 0.0001f ? Vector2(x / len, y / len) : Vector2(0, 0);
    }

    float angle() const { return atan2(y, x) * 180.0f / PI; }
};

// ============================================================================
// PARTICLE SYSTEM
// ============================================================================

struct Particle {
    Vector2 position;
    Vector2 velocity;
    sf::Color color;
    float lifetime;
    float maxLifetime;
    float size;
    float rotation;
    float rotationSpeed;

    Particle(const Vector2& pos, const Vector2& vel, const sf::Color& col, float life, float sz)
        : position(pos), velocity(vel), color(col), lifetime(life), maxLifetime(life), size(sz),
        rotation(0), rotationSpeed(RandomGenerator::range(-180.0f, 180.0f)) {}

    void update(float dt) {
        position = position + velocity * dt;
        velocity = velocity * 0.98f;
        lifetime -= dt;
        rotation += rotationSpeed * dt;
        float alpha = (lifetime / maxLifetime) * 255.0f;
        color.a = static_cast<sf::Uint8>(max(0.0f, alpha));
    }

    bool isAlive() const { return lifetime > 0; }
};

class ParticleSystem {
private:
    deque<Particle> particles;
    int maxParticles;

public:
    ParticleSystem(int max = 500) : maxParticles(max) {}

    void emit(const Vector2& pos, const Vector2& baseVel, const sf::Color& color, int count, float life, float size = 3.0f) {
        for (int i = 0; i < count && particles.size() < static_cast<size_t>(maxParticles); i++) {
            Vector2 vel = baseVel + Vector2(RandomGenerator::range(-80.0f, 80.0f), RandomGenerator::range(-80.0f, 80.0f));
            particles.emplace_back(pos, vel, color, life + RandomGenerator::range(-0.2f, 0.2f), size);
        }
    }

    void emitExplosion(const Vector2& pos, int count, float size) {
        for (int i = 0; i < count && particles.size() < static_cast<size_t>(maxParticles); i++) {
            float angle = RandomGenerator::range(0.0f, 360.0f) * PI / 180.0f;
            float speed = RandomGenerator::range(50.0f, 200.0f);
            Vector2 vel(cos(angle) * speed, sin(angle) * speed);
            sf::Color col;
            int colorType = RandomGenerator::range(0, 3);
            if (colorType == 0) col = sf::Color(255, 200, 50);
            else if (colorType == 1) col = sf::Color(255, 100, 0);
            else col = sf::Color(255, 50, 0);
            particles.emplace_back(pos, vel, col, RandomGenerator::range(0.5f, 1.2f), size);
        }
    }

    void update(float dt) {
        for (auto& p : particles) p.update(dt);
        while (!particles.empty() && !particles.front().isAlive()) particles.pop_front();
    }

    void draw(sf::RenderWindow& window) {
        for (auto& p : particles) {
            sf::CircleShape shape(p.size);
            shape.setFillColor(p.color);
            shape.setOrigin(p.size, p.size);
            shape.setPosition(p.position.x, p.position.y);
            shape.setRotation(p.rotation);
            window.draw(shape);
        }
    }

    void clear() { particles.clear(); }
    size_t getCount() const { return particles.size(); }
};

// ============================================================================
// STARFIELD BACKGROUND
// ============================================================================

class Starfield {
private:
    struct Star {
        float x, y, speed, brightness, size;
    };
    vector<Star> stars;

public:
    Starfield(int count = 200) {
        for (int i = 0; i < count; i++) {
            Star s;
            s.x = RandomGenerator::range(0.0f, SCREEN_WIDTH);
            s.y = RandomGenerator::range(0.0f, SCREEN_HEIGHT);
            s.speed = RandomGenerator::range(20.0f, 100.0f);
            s.brightness = RandomGenerator::range(100.0f, 255.0f);
            s.size = RandomGenerator::range(1.0f, 2.5f);
            stars.push_back(s);
        }
    }

    void update(float dt) {
        for (auto& s : stars) {
            s.y += s.speed * dt;
            if (s.y > SCREEN_HEIGHT) {
                s.y = -5;
                s.x = RandomGenerator::range(0.0f, SCREEN_WIDTH);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto& s : stars) {
            sf::CircleShape star(s.size);
            star.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(s.brightness)));
            star.setPosition(s.x, s.y);
            window.draw(star);
        }
    }
};

// ============================================================================
// ENUMS
// ============================================================================

enum class GameScreen { Intro, Menu, Instructions, Gameplay, Pause, HighScore, GameOver, Victory, BossWarning };
enum class EnemyType { Alpha, Beta, Gamma, Monster, Phantom, Dragon };
enum class PowerUpType { Power, Fire, Shield, Lives, Nuke, MultiShot, Slow, Danger };

// ============================================================================
// GAME OBJECT BASE CLASS
// ============================================================================

class GameObject {
protected:
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool active;
    sf::Sprite sprite;
    float boundingRadius;
    float health;
    float maxHealth;

public:
    GameObject() : position(0, 0), velocity(0, 0), rotation(0), active(true), boundingRadius(20), health(100), maxHealth(100) {}
    virtual ~GameObject() = default;

    virtual void update(float dt) {
        position = position + velocity * dt;
        sprite.setPosition(position.x, position.y);
        sprite.setRotation(rotation);
    }

    virtual void draw(sf::RenderWindow& window) {
        if (active) window.draw(sprite);
    }

    virtual void drawHealthBar(sf::RenderWindow& window, float width = 40.0f, float offsetY = -30.0f) {
        if (!active || health >= maxHealth) return;

        float barHeight = 6.0f;
        float healthPercent = health / maxHealth;

        // Background
        sf::RectangleShape bg(sf::Vector2f(width, barHeight));
        bg.setPosition(position.x - width / 2, position.y + offsetY);
        bg.setFillColor(sf::Color(60, 60, 60));
        bg.setOutlineColor(sf::Color::Black);
        bg.setOutlineThickness(1);
        window.draw(bg);

        // Health bar with color gradient
        sf::Color healthColor;
        if (healthPercent > 0.6f) healthColor = sf::Color(50, 205, 50);
        else if (healthPercent > 0.3f) healthColor = sf::Color(255, 200, 0);
        else healthColor = sf::Color(220, 50, 50);

        sf::RectangleShape bar(sf::Vector2f(width * healthPercent, barHeight));
        bar.setPosition(position.x - width / 2, position.y + offsetY);
        bar.setFillColor(healthColor);
        window.draw(bar);
    }

    bool checkCollision(GameObject* other) const {
        if (!active || !other->isActive()) return false;
        return position.distanceTo(other->getPosition()) < (boundingRadius + other->getBoundingRadius());
    }

    // Accessors
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }
    Vector2 getPosition() const { return position; }
    void setPosition(const Vector2& p) { position = p; sprite.setPosition(p.x, p.y); }
    Vector2 getVelocity() const { return velocity; }
    void setVelocity(const Vector2& v) { velocity = v; }
    float getRotation() const { return rotation; }
    void setRotation(float r) { rotation = r; sprite.setRotation(r); }
    float getBoundingRadius() const { return boundingRadius; }
    void setBoundingRadius(float r) { boundingRadius = r; }
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    void setHealth(float h) { health = h; }
    void setMaxHealth(float h) { maxHealth = h; }

    void setupSprite(const string& textureName, float scale = 1.0f) {
        TextureManager& tm = TextureManager::getInstance();
        if (tm.hasTexture(textureName)) {
            sprite.setTexture(tm.getTexture(textureName));
            sprite.setOrigin(sprite.getTexture()->getSize().x / 2.0f, sprite.getTexture()->getSize().y / 2.0f);
            sprite.setScale(scale, scale);
            boundingRadius = (sprite.getTexture()->getSize().x * scale) / 2.5f;
        }
    }
};

// ============================================================================
// BULLET CLASS
// ============================================================================

class Bullet : public GameObject {
private:
    bool fromPlayer;
    int damage;
    bool isBossBullet;

public:
    Bullet(bool playerBullet, int dmg = 10, bool bossBullet = false)
        : fromPlayer(playerBullet), damage(dmg), isBossBullet(bossBullet) {

        if (bossBullet) {
            setupSprite("boss_bullet", 1.2f);
        }
        else if (playerBullet) {
            setupSprite("player_bullet", 0.9f);
        }
        else {
            setupSprite("enemy_bullet", 0.7f);
        }
        boundingRadius = 8.0f;
    }

    void update(float dt) override {
        GameObject::update(dt);
        if (position.x < -50 || position.x > SCREEN_WIDTH + 50 ||
            position.y < -50 || position.y > SCREEN_HEIGHT + 50) {
            active = false;
        }
    }

    bool isFromPlayer() const { return fromPlayer; }
    int getDamage() const { return damage; }
    bool isBoss() const { return isBossBullet; }
};

// ============================================================================
// PLAYER SPACESHIP CLASS
// ============================================================================

class Spaceship : public GameObject {
private:
    float shield;
    float maxShield;
    float fireRate;
    float fireTimer;
    float invincibilityTimer;
    bool isInvincible;
    int lives;
    int score;
    int combo;
    float comboTimer;
    int powerLevel;
    int multiShotLevel;
    bool hasShield;
    ParticleSystem thrustParticles;
    sf::Sprite shieldSprite;

public:
    Spaceship() : shield(0), maxShield(PLAYER_MAX_SHIELD), fireRate(BASE_FIRE_RATE), fireTimer(0),
        invincibilityTimer(0), isInvincible(false), lives(3), score(0), combo(0),
        comboTimer(0), powerLevel(1), multiShotLevel(1), hasShield(false) {

        setupSprite("spaceship", 1.2f);
        health = PLAYER_MAX_HEALTH;
        maxHealth = PLAYER_MAX_HEALTH;

        TextureManager& tm = TextureManager::getInstance();
        if (tm.hasTexture("shield_effect")) {
            shieldSprite.setTexture(tm.getTexture("shield_effect"));
            shieldSprite.setOrigin(shieldSprite.getTexture()->getSize().x / 2.0f,
                shieldSprite.getTexture()->getSize().y / 2.0f);
            shieldSprite.setScale(1.2f, 1.2f);
        }
    }

    void update(float dt) override {
        GameObject::update(dt);

        // Clamp position to screen
        position.x = max(30.0f, min(SCREEN_WIDTH - 30.0f, position.x));
        position.y = max(30.0f, min(SCREEN_HEIGHT - 30.0f, position.y));

        fireTimer -= dt;

        if (isInvincible) {
            invincibilityTimer -= dt;
            if (invincibilityTimer <= 0) isInvincible = false;
        }

        // Combo timer
        if (combo > 0) {
            comboTimer -= dt;
            if (comboTimer <= 0) combo = 0;
        }

        // Thrust particles
        if (velocity.length() > 30.0f) {
            thrustParticles.emit(position + Vector2(0, 25), Vector2(0, 50),
                sf::Color(100, 150, 255, 200), 1, 0.3f, 2.0f);
        }
        thrustParticles.update(dt);

        sprite.setPosition(position.x, position.y);
        shieldSprite.setPosition(position.x, position.y);
    }

    void draw(sf::RenderWindow& window) override {
        thrustParticles.draw(window);

        if (isInvincible) {
            float alpha = 150 + sin(invincibilityTimer * 15) * 100;
            sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
        }
        else {
            sprite.setColor(sf::Color::White);
        }

        GameObject::draw(window);

        if (hasShield && shield > 0) {
            float shieldAlpha = (shield / maxShield) * 180.0f;
            shieldSprite.setColor(sf::Color(100, 200, 255, static_cast<sf::Uint8>(shieldAlpha)));
            window.draw(shieldSprite);
        }
    }

    void drawHUD(sf::RenderWindow& window, sf::Font& font) {
        // Health bar
        float barWidth = 200.0f;
        float barHeight = 20.0f;
        float startX = 20.0f;
        float startY = SCREEN_HEIGHT - 50.0f;

        // Health background
        sf::RectangleShape healthBg(sf::Vector2f(barWidth, barHeight));
        healthBg.setPosition(startX, startY);
        healthBg.setFillColor(sf::Color(40, 40, 40));
        healthBg.setOutlineColor(sf::Color::White);
        healthBg.setOutlineThickness(2);
        window.draw(healthBg);

        // Health fill
        float healthPercent = health / maxHealth;
        sf::Color healthColor = healthPercent > 0.5f ? sf::Color(50, 200, 50) :
            healthPercent > 0.25f ? sf::Color(255, 200, 0) : sf::Color(220, 50, 50);
        sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercent, barHeight));
        healthBar.setPosition(startX, startY);
        healthBar.setFillColor(healthColor);
        window.draw(healthBar);

        // Health text
        sf::Text healthText;
        healthText.setFont(font);
        healthText.setString("HP: " + to_string(static_cast<int>(health)) + "/" + to_string(static_cast<int>(maxHealth)));
        healthText.setCharacterSize(14);
        healthText.setFillColor(sf::Color::White);
        healthText.setPosition(startX + 5, startY + 2);
        window.draw(healthText);

        // Shield bar (if active)
        if (hasShield) {
            sf::RectangleShape shieldBg(sf::Vector2f(barWidth, 10.0f));
            shieldBg.setPosition(startX, startY - 15);
            shieldBg.setFillColor(sf::Color(20, 20, 60));
            shieldBg.setOutlineColor(sf::Color(100, 150, 255));
            shieldBg.setOutlineThickness(1);
            window.draw(shieldBg);

            sf::RectangleShape shieldBar(sf::Vector2f(barWidth * (shield / maxShield), 10.0f));
            shieldBar.setPosition(startX, startY - 15);
            shieldBar.setFillColor(sf::Color(100, 180, 255));
            window.draw(shieldBar);
        }

        // Lives
        sf::Text livesText;
        livesText.setFont(font);
        livesText.setString("Lives: " + to_string(lives));
        livesText.setCharacterSize(20);
        livesText.setFillColor(sf::Color(255, 100, 100));
        livesText.setPosition(startX, startY - 40 - (hasShield ? 15 : 0));
        window.draw(livesText);

        // Score
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color(255, 220, 100));
        scoreText.setPosition(20, 20);
        window.draw(scoreText);

        // Combo
        if (combo > 1) {
            sf::Text comboText;
            comboText.setFont(font);
            comboText.setString("COMBO x" + to_string(combo));
            comboText.setCharacterSize(28);
            comboText.setFillColor(sf::Color(255, 150, 50));
            comboText.setPosition(20, 50);
            window.draw(comboText);
        }

        // Power level
        sf::Text powerText;
        powerText.setFont(font);
        powerText.setString("Power: " + to_string(powerLevel) + " | Shots: " + to_string(multiShotLevel));
        powerText.setCharacterSize(16);
        powerText.setFillColor(sf::Color(150, 200, 255));
        powerText.setPosition(20, 85);
        window.draw(powerText);
    }

    bool canFire() const { return fireTimer <= 0; }
    void resetFireTimer() { fireTimer = fireRate; }

    void takeDamage(float dmg) {
        if (isInvincible) return;

        if (hasShield && shield > 0) {
            shield -= dmg;
            if (shield <= 0) {
                shield = 0;
                hasShield = false;
            }
            return;
        }

        health -= dmg;
        SoundManager::getInstance().playSound("player_hurt");

        if (health <= 0) {
            lives--;
            if (lives > 0) {
                health = maxHealth;
                isInvincible = true;
                invincibilityTimer = 3.0f;
            }
        }
    }

    void addScore(int points) {
        combo++;
        comboTimer = 2.0f;
        int comboMultiplier = min(combo, 10);
        score += points * comboMultiplier;
    }

    void applyPowerUp(PowerUpType type) {
        SoundManager::getInstance().playSound("powerup");
        switch (type) {
        case PowerUpType::Power:
            powerLevel = min(powerLevel + 1, 5);
            fireRate = max(fireRate - 0.015f, MIN_FIRE_RATE);
            break;
        case PowerUpType::Fire:
            fireRate = max(fireRate - 0.025f, MIN_FIRE_RATE);
            break;
        case PowerUpType::Shield:
            hasShield = true;
            shield = maxShield;
            break;
        case PowerUpType::Lives:
            lives++;
            health = min(health + 30.0f, maxHealth);
            break;
        case PowerUpType::MultiShot:
            multiShotLevel = min(multiShotLevel + 1, 5);
            break;
        case PowerUpType::Danger:
            isInvincible = true;
            invincibilityTimer = 5.0f;
            break;
        default:
            break;
        }
    }

    void reset() {
        health = maxHealth;
        shield = 0;
        hasShield = false;
        fireRate = BASE_FIRE_RATE;
        lives = 3;
        score = 0;
        combo = 0;
        powerLevel = 1;
        multiShotLevel = 1;
        isInvincible = false;
        position = Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100);
        velocity = Vector2(0, 0);
        active = true;
        thrustParticles.clear();
    }

    int getLives() const { return lives; }
    int getScore() const { return score; }
    int getPowerLevel() const { return powerLevel; }
    int getMultiShotLevel() const { return multiShotLevel; }
    bool getIsInvincible() const { return isInvincible; }
    float getFireRate() const { return fireRate; }
};

// ============================================================================
// ENEMY BASE CLASS
// ============================================================================

class Enemy : public GameObject {
protected:
    EnemyType type;
    int scoreValue;
    float fireRate;
    float fireTimer;
    int level;
    int phase;
    Vector2 playerPos;

public:
    Enemy(EnemyType t, int lvl, int ph) : type(t), scoreValue(100), fireRate(2.0f), fireTimer(0), level(lvl), phase(ph) {
        fireTimer = RandomGenerator::range(1.0f, 3.0f);
    }

    virtual void update(float dt) override {
        GameObject::update(dt);
        fireTimer -= dt;

        // Remove if off screen
        if (position.y > SCREEN_HEIGHT + 100) active = false;
    }

    void draw(sf::RenderWindow& window) override {
        GameObject::draw(window);
        drawHealthBar(window, 35.0f, -boundingRadius - 10);
    }

    void takeDamage(float dmg) {
        health -= dmg;
        if (health <= 0) active = false;
    }

    bool canFire() const { return fireTimer <= 0 && position.y > 50 && position.y < SCREEN_HEIGHT - 100; }
    void resetFireTimer() { fireTimer = fireRate + RandomGenerator::range(-0.5f, 0.5f); }
    void setPlayerPosition(const Vector2& pos) { playerPos = pos; }
    int getScoreValue() const { return scoreValue; }
    EnemyType getType() const { return type; }
};

// ============================================================================
// SPECIFIC ENEMY TYPES
// ============================================================================

// Alpha Enemy - Basic, moves straight down
class AlphaEnemy : public Enemy {
public:
    AlphaEnemy(int level, int phase) : Enemy(EnemyType::Alpha, level, phase) {
        setupSprite("enemy_alpha", 1.2f);
        health = 30 + level * 10;
        maxHealth = health;
        scoreValue = 100 + level * 20;
        velocity = Vector2(RandomGenerator::range(-30.0f, 30.0f), RandomGenerator::range(80.0f, 120.0f));
        fireRate = 2.5f;
    }
};

// Beta Enemy - Weaves side to side
class BetaEnemy : public Enemy {
private:
    float waveTimer;
    float waveAmplitude;

public:
    BetaEnemy(int level, int phase) : Enemy(EnemyType::Beta, level, phase), waveTimer(0) {
        setupSprite("enemy_beta", 1.0f);
        health = 45 + level * 15;
        maxHealth = health;
        scoreValue = 150 + level * 30;
        velocity = Vector2(0, RandomGenerator::range(60.0f, 100.0f));
        waveAmplitude = RandomGenerator::range(80.0f, 150.0f);
        fireRate = 2.0f;
    }

    void update(float dt) override {
        waveTimer += dt * 3.0f;
        position.x += sin(waveTimer) * waveAmplitude * dt;
        Enemy::update(dt);
    }
};

// Gamma Enemy - Follows player horizontally
class GammaEnemy : public Enemy {
private:
    float seekSpeed;

public:
    GammaEnemy(int level, int phase) : Enemy(EnemyType::Gamma, level, phase) {
        setupSprite("enemy_gamma", 0.9f);
        health = 60 + level * 20;
        maxHealth = health;
        scoreValue = 200 + level * 40;
        velocity = Vector2(0, RandomGenerator::range(40.0f, 70.0f));
        seekSpeed = 100.0f + level * 20.0f;
        fireRate = 1.8f;
    }

    void update(float dt) override {
        // Move towards player horizontally
        if (playerPos.x > position.x + 20) velocity.x = seekSpeed;
        else if (playerPos.x < position.x - 20) velocity.x = -seekSpeed;
        else velocity.x = 0;
        Enemy::update(dt);
    }
};

// Monster Enemy - Charges at player
class MonsterEnemy : public Enemy {
private:
    float chargeTimer;
    bool isCharging;

public:
    MonsterEnemy(int level, int phase) : Enemy(EnemyType::Monster, level, phase), chargeTimer(3.0f), isCharging(false) {
        setupSprite("enemy_monster", 0.95f);
        health = 80 + level * 25;
        maxHealth = health;
        scoreValue = 300 + level * 50;
        velocity = Vector2(RandomGenerator::range(-20.0f, 20.0f), 50.0f);
        fireRate = 1.5f;
    }

    void update(float dt) override {
        chargeTimer -= dt;

        if (!isCharging && chargeTimer <= 0) {
            Vector2 dir = (playerPos - position).normalized();
            velocity = dir * 300.0f;
            isCharging = true;
            chargeTimer = 1.5f;
        }
        else if (isCharging && chargeTimer <= 0) {
            velocity = Vector2(RandomGenerator::range(-20.0f, 20.0f), 50.0f);
            isCharging = false;
            chargeTimer = RandomGenerator::range(2.0f, 4.0f);
        }

        Enemy::update(dt);
    }
};

// Phantom Enemy - Fades in and out
class PhantomEnemy : public Enemy {
private:
    float fadeTimer;
    bool isVisible;

public:
    PhantomEnemy(int level, int phase) : Enemy(EnemyType::Phantom, level, phase), fadeTimer(2.0f), isVisible(true) {
        setupSprite("enemy_phantom", 0.8f);
        health = 50 + level * 15;
        maxHealth = health;
        scoreValue = 250 + level * 45;
        velocity = Vector2(RandomGenerator::range(-50.0f, 50.0f), RandomGenerator::range(70.0f, 100.0f));
        fireRate = 1.3f;
    }

    void update(float dt) override {
        fadeTimer -= dt;
        if (fadeTimer <= 0) {
            isVisible = !isVisible;
            fadeTimer = 2.0f;
        }

        sf::Uint8 alpha = isVisible ? 255 : 80;
        sprite.setColor(sf::Color(255, 255, 255, alpha));

        Enemy::update(dt);
    }
};

// Dragon Enemy - Mini-boss, complex movement
class DragonEnemy : public Enemy {
private:
    float stateTimer;
    int state;
    float angleOffset;

public:
    DragonEnemy(int level, int phase) : Enemy(EnemyType::Dragon, level, phase), stateTimer(3.0f), state(0), angleOffset(0) {
        setupSprite("enemy_dragon", 1.0f);
        health = 200 + level * 50;
        maxHealth = health;
        scoreValue = 500 + level * 100;
        velocity = Vector2(0, 0);
        fireRate = 0.5f;
        boundingRadius = 40.0f;
    }

    void update(float dt) override {
        stateTimer -= dt;
        angleOffset += dt;

        // Circle movement
        position.x = SCREEN_WIDTH / 2 + cos(angleOffset) * 200;
        position.y = 150 + sin(angleOffset * 0.5f) * 50;

        sprite.setPosition(position.x, position.y);
        fireTimer -= dt;

        if (health <= 0) active = false;
    }

    void draw(sf::RenderWindow& window) override {
        GameObject::draw(window);
        drawHealthBar(window, 60.0f, -50.0f);
    }
};

// ============================================================================
// FINAL BOSS CLASS
// ============================================================================

class FinalBoss : public GameObject {
private:
    int bossPhase;
    float phaseTimer;
    float attackTimer;
    float moveAngle;
    Vector2 playerPos;
    sf::Sprite eyeSprite;
    int attackPattern;
    vector<unique_ptr<Bullet>> pendingBullets;
    bool isEnraged;
    float shieldTimer;
    bool hasShield;

public:
    FinalBoss() : bossPhase(1), phaseTimer(0), attackTimer(0), moveAngle(0), attackPattern(0), isEnraged(false), shieldTimer(0), hasShield(true) {
        setupSprite("boss", 1.5f);
        health = 500.0f;
        maxHealth = 500.0f;
        boundingRadius = 80.0f;
        position = Vector2(SCREEN_WIDTH / 2, -150);

        TextureManager& tm = TextureManager::getInstance();
        if (tm.hasTexture("boss_eye")) {
            eyeSprite.setTexture(tm.getTexture("boss_eye"));
            eyeSprite.setOrigin(eyeSprite.getTexture()->getSize().x / 2.0f,
                eyeSprite.getTexture()->getSize().y / 2.0f);
        }
    }

    void update(float dt) override {
        // Entry animation
        if (position.y < 150) {
            position.y += 50.0f * dt;
            sprite.setPosition(position.x, position.y);
            eyeSprite.setPosition(position.x, position.y - 10);
            return;
        }

        attackTimer -= dt;
        moveAngle += dt;
        phaseTimer += dt;
        shieldTimer -= dt;

        // Update boss phase based on health
        float healthPercent = health / maxHealth;
        if (healthPercent <= 0.3f) {
            bossPhase = 3;
            isEnraged = true;
        }
        else if (healthPercent <= 0.6f) {
            bossPhase = 2;
        }

        // Movement patterns
        switch (bossPhase) {
        case 1:
            position.x = SCREEN_WIDTH / 2 + sin(moveAngle * 0.8f) * 250;
            position.y = 150 + sin(moveAngle * 0.5f) * 30;
            break;
        case 2:
            position.x = SCREEN_WIDTH / 2 + sin(moveAngle * 1.2f) * 300;
            position.y = 120 + sin(moveAngle * 0.8f) * 50;
            break;
        case 3:
            position.x = SCREEN_WIDTH / 2 + sin(moveAngle * 1.5f) * 350;
            position.y = 100 + sin(moveAngle) * 80;
            break;
        }

        // Shield regeneration
        if (shieldTimer <= 0 && !hasShield && bossPhase >= 2) {
            hasShield = true;
            shieldTimer = 10.0f;
        }

        sprite.setPosition(position.x, position.y);

        // Eye follows player
        Vector2 toPlayer = (playerPos - position).normalized();
        eyeSprite.setPosition(position.x + toPlayer.x * 15, position.y - 10 + toPlayer.y * 10);

        // Enraged color
        if (isEnraged) {
            float pulse = sin(phaseTimer * 8) * 0.3f + 0.7f;
            sprite.setColor(sf::Color(255, static_cast<sf::Uint8>(100 * pulse), static_cast<sf::Uint8>(100 * pulse)));
        }
    }

    void draw(sf::RenderWindow& window) override {
        // Draw shield if active
        if (hasShield) {
            sf::CircleShape shieldCircle(boundingRadius + 20);
            shieldCircle.setOrigin(boundingRadius + 20, boundingRadius + 20);
            shieldCircle.setPosition(position.x, position.y);
            shieldCircle.setFillColor(sf::Color(100, 150, 255, 60));
            shieldCircle.setOutlineColor(sf::Color(150, 200, 255, 150));
            shieldCircle.setOutlineThickness(3);
            window.draw(shieldCircle);
        }

        GameObject::draw(window);
        window.draw(eyeSprite);
    }

    void drawBossHealthBar(sf::RenderWindow& window, sf::Font& font) {
        float barWidth = 500.0f;
        float barHeight = 30.0f;
        float startX = (SCREEN_WIDTH - barWidth) / 2;
        float startY = 20.0f;

        // Boss name
        sf::Text bossName;
        bossName.setFont(font);
        bossName.setString("EMPEROR DESTRUCTON" + string(isEnraged ? " [ENRAGED]" : ""));
        bossName.setCharacterSize(24);
        bossName.setFillColor(isEnraged ? sf::Color(255, 100, 100) : sf::Color(255, 200, 100));
        bossName.setPosition(startX, startY - 30);
        window.draw(bossName);

        // Health bar background
        sf::RectangleShape bg(sf::Vector2f(barWidth, barHeight));
        bg.setPosition(startX, startY);
        bg.setFillColor(sf::Color(40, 0, 0));
        bg.setOutlineColor(sf::Color(200, 50, 50));
        bg.setOutlineThickness(3);
        window.draw(bg);

        // Health bar fill
        float healthPercent = health / maxHealth;
        sf::Color barColor;
        if (bossPhase == 1) barColor = sf::Color(200, 50, 50);
        else if (bossPhase == 2) barColor = sf::Color(255, 150, 0);
        else barColor = sf::Color(255, 50, 150);

        sf::RectangleShape bar(sf::Vector2f(barWidth * healthPercent, barHeight));
        bar.setPosition(startX, startY);
        bar.setFillColor(barColor);
        window.draw(bar);

        // Phase indicator
        sf::Text phaseText;
        phaseText.setFont(font);
        phaseText.setString("Phase " + to_string(bossPhase) + "/3");
        phaseText.setCharacterSize(16);
        phaseText.setFillColor(sf::Color::White);
        phaseText.setPosition(startX + barWidth + 10, startY + 5);
        window.draw(phaseText);
    }

    void takeDamage(float dmg) {
        if (hasShield) {
            dmg *= 0.3f;
            shieldTimer -= 2.0f;
            if (shieldTimer <= 0) hasShield = false;
        }
        health -= dmg;
        SoundManager::getInstance().playSound("boss_hit");
        if (health <= 0) active = false;
    }

    vector<unique_ptr<Bullet>> getAttackBullets() {
        vector<unique_ptr<Bullet>> bullets;

        if (attackTimer > 0 || position.y < 150) return bullets;

        float attackDelay;
        switch (bossPhase) {
        case 1: attackDelay = 0.8f; break;
        case 2: attackDelay = 0.5f; break;
        case 3: attackDelay = 0.3f; break;
        default: attackDelay = 1.0f;
        }
        attackTimer = attackDelay;
        attackPattern = (attackPattern + 1) % 4;

        // Different attack patterns
        switch (attackPattern) {
        case 0: // Spread shot
        {
            int bulletCount = 5 + bossPhase * 2;
            float angleSpread = 60.0f + bossPhase * 20.0f;
            float startAngle = 90.0f - angleSpread / 2;

            for (int i = 0; i < bulletCount; i++) {
                float angle = (startAngle + (angleSpread / (bulletCount - 1)) * i) * PI / 180.0f;
                auto bullet = make_unique<Bullet>(false, 15 + bossPhase * 5, true);
                bullet->setPosition(position + Vector2(0, 40));
                bullet->setVelocity(Vector2(cos(angle) * 250, sin(angle) * 250));
                bullets.push_back(move(bullet));
            }
            break;
        }
        case 1: // Aimed shot
        {
            Vector2 dir = (playerPos - position).normalized();
            for (int i = -1; i <= 1; i++) {
                auto bullet = make_unique<Bullet>(false, 20 + bossPhase * 5, true);
                bullet->setPosition(position + Vector2(i * 30, 40));
                bullet->setVelocity(dir * 350.0f);
                bullets.push_back(move(bullet));
            }
            break;
        }
        case 2: // Circular burst
        {
            int bulletCount = 8 + bossPhase * 4;
            for (int i = 0; i < bulletCount; i++) {
                float angle = (360.0f / bulletCount * i + phaseTimer * 30) * PI / 180.0f;
                auto bullet = make_unique<Bullet>(false, 10 + bossPhase * 3, true);
                bullet->setPosition(position);
                bullet->setVelocity(Vector2(cos(angle) * 200, sin(angle) * 200));
                bullets.push_back(move(bullet));
            }
            break;
        }
        case 3: // Spiral
        {
            if (bossPhase >= 2) {
                for (int i = 0; i < 3; i++) {
                    float angle = (phaseTimer * 100 + i * 120) * PI / 180.0f;
                    auto bullet = make_unique<Bullet>(false, 12 + bossPhase * 4, true);
                    bullet->setPosition(position);
                    bullet->setVelocity(Vector2(cos(angle) * 220, sin(angle) * 220));
                    bullets.push_back(move(bullet));
                }
            }
            break;
        }
        }

        return bullets;
    }

    void setPlayerPosition(const Vector2& pos) { playerPos = pos; }
    int getBossPhase() const { return bossPhase; }
    bool hasActiveShield() const { return hasShield; }
};

// ============================================================================
// POWER-UP CLASS
// ============================================================================

class PowerUp : public GameObject {
private:
    PowerUpType type;
    float lifetime;
    float bobTimer;

public:
    PowerUp(PowerUpType t) : type(t), lifetime(12.0f), bobTimer(0) {
        string textureName;
        switch (type) {
        case PowerUpType::Power: textureName = "powerup_power"; break;
        case PowerUpType::Fire: textureName = "powerup_fire"; break;
        case PowerUpType::Shield: textureName = "powerup_shield"; break;
        case PowerUpType::Lives: textureName = "powerup_lives"; break;
        case PowerUpType::Nuke: textureName = "powerup_nuke"; break;
        case PowerUpType::MultiShot: textureName = "powerup_multishot"; break;
        case PowerUpType::Slow: textureName = "powerup_slow"; break;
        case PowerUpType::Danger: textureName = "powerup_danger"; break;
        }
        setupSprite(textureName, 1.0f);
        velocity = Vector2(RandomGenerator::range(-20.0f, 20.0f), RandomGenerator::range(40.0f, 80.0f));
        boundingRadius = 15.0f;
    }

    void update(float dt) override {
        GameObject::update(dt);
        lifetime -= dt;
        bobTimer += dt;
        rotation += 90.0f * dt;

        // Bob up and down
        sprite.setPosition(position.x, position.y + sin(bobTimer * 4) * 5);
        sprite.setRotation(rotation);

        if (lifetime <= 0 || position.y > SCREEN_HEIGHT + 50) {
            active = false;
        }

        // Blink when about to expire
        if (lifetime < 3.0f) {
            float alpha = 128 + sin(lifetime * 10) * 127;
            sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
        }
    }

    PowerUpType getType() const { return type; }
};

// ============================================================================
// EXPLOSION ANIMATION
// ============================================================================

class Explosion : public GameObject {
private:
    int currentFrame;
    int totalFrames;
    float frameTime;
    float frameTimer;
    float frameWidth;

public:
    Explosion(const Vector2& pos, float scale = 1.0f) : currentFrame(0), totalFrames(8), frameTime(0.08f), frameTimer(0) {
        position = pos;
        TextureManager& tm = TextureManager::getInstance();
        if (tm.hasTexture("explosion")) {
            sprite.setTexture(tm.getTexture("explosion"));
            frameWidth = sprite.getTexture()->getSize().x / totalFrames;
            sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(frameWidth), sprite.getTexture()->getSize().y));
            sprite.setOrigin(frameWidth / 2, sprite.getTexture()->getSize().y / 2.0f);
            sprite.setScale(scale, scale);
            sprite.setPosition(pos.x, pos.y);
        }
    }

    void update(float dt) override {
        frameTimer += dt;
        if (frameTimer >= frameTime) {
            frameTimer = 0;
            currentFrame++;
            if (currentFrame >= totalFrames) {
                active = false;
            }
            else {
                sprite.setTextureRect(sf::IntRect(static_cast<int>(currentFrame * frameWidth), 0,
                    static_cast<int>(frameWidth), sprite.getTexture()->getSize().y));
            }
        }
    }
};

// ============================================================================
// GAME STATE CLASS
// ============================================================================

class GameState {
private:
    // Core game objects
    unique_ptr<Spaceship> player;
    unique_ptr<FinalBoss> boss;
    vector<unique_ptr<Enemy>> enemies;
    vector<unique_ptr<Bullet>> bullets;
    vector<unique_ptr<PowerUp>> powerUps;
    vector<unique_ptr<Explosion>> explosions;

    // Visual effects
    unique_ptr<Starfield> starfield;
    ParticleSystem particles;

    // Backgrounds
    sf::Sprite gameBackground;
    sf::Sprite menuBackground;
    sf::Sprite bossBackground;
    sf::Sprite introSprite;
    sf::Sprite logoSprite;

    // Game state
    GameScreen currentScreen;
    int currentLevel;
    int currentPhase;
    float phaseTimer;
    bool isBossLevel;

    // Intro
    float introTimer;
    int introFrame;
    vector<string> introTexts;
    int currentIntroText;

    // Timing
    sf::Clock gameClock;
    float deltaTime;
    float slowTimeMultiplier;
    float slowTimeTimer;

    // Screen shake
    float shakeIntensity;
    float shakeTimer;
    Vector2 shakeOffset;

    // UI
    sf::Font gameFont;
    bool fontLoaded;

    // High scores
    vector<pair<string, int>> highScores;
    string playerName;

    // Settings
    bool soundEnabled;
    float difficulty;

    // Input tracking
    bool mKeyPressed;
    bool pKeyPressed;

public:
    GameState() : currentScreen(GameScreen::Intro), currentLevel(1), currentPhase(1),
        phaseTimer(30.0f), isBossLevel(false), introTimer(0), introFrame(0),
        currentIntroText(0), deltaTime(0), slowTimeMultiplier(1.0f), slowTimeTimer(0),
        shakeIntensity(0), shakeTimer(0), fontLoaded(false), soundEnabled(true),
        difficulty(1.0f), mKeyPressed(false), pKeyPressed(false) {

        RandomGenerator::seed();

        // Load all resources
        TextureManager::getInstance().loadAllTextures();
        SoundManager::getInstance().loadAllSounds();

        // Load font
        fontLoaded = gameFont.loadFromFile("assets/font.otf");
        if (!fontLoaded) {
            // Try system fonts
            vector<string> fontPaths = {
                "C:/Windows/Fonts/arial.ttf",
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
                "/System/Library/Fonts/Helvetica.ttc"
            };
            for (const auto& path : fontPaths) {
                if (gameFont.loadFromFile(path)) {
                    fontLoaded = true;
                    break;
                }
            }
        }

        // Setup backgrounds
        TextureManager& tm = TextureManager::getInstance();
        if (tm.hasTexture("background")) {
            gameBackground.setTexture(tm.getTexture("background"));
            float scaleX = SCREEN_WIDTH / gameBackground.getTexture()->getSize().x;
            float scaleY = SCREEN_HEIGHT / gameBackground.getTexture()->getSize().y;
            gameBackground.setScale(scaleX, scaleY);
        }
        if (tm.hasTexture("menu_background")) {
            menuBackground.setTexture(tm.getTexture("menu_background"));
            float scaleX = SCREEN_WIDTH / menuBackground.getTexture()->getSize().x;
            float scaleY = SCREEN_HEIGHT / menuBackground.getTexture()->getSize().y;
            menuBackground.setScale(scaleX, scaleY);
        }
        if (tm.hasTexture("boss_background")) {
            bossBackground.setTexture(tm.getTexture("boss_background"));
            float scaleX = SCREEN_WIDTH / bossBackground.getTexture()->getSize().x;
            float scaleY = SCREEN_HEIGHT / bossBackground.getTexture()->getSize().y;
            bossBackground.setScale(scaleX, scaleY);
        }
        if (tm.hasTexture("intro_video")) {
            introSprite.setTexture(tm.getTexture("intro_video"));
            float scaleX = SCREEN_WIDTH / introSprite.getTexture()->getSize().x;
            float scaleY = SCREEN_HEIGHT / introSprite.getTexture()->getSize().y;
            introSprite.setScale(scaleX, scaleY);
        }
        if (tm.hasTexture("logo")) {
            logoSprite.setTexture(tm.getTexture("logo"));
            logoSprite.setOrigin(logoSprite.getTexture()->getSize().x / 2.0f,
                logoSprite.getTexture()->getSize().y / 2.0f);
            logoSprite.setPosition(SCREEN_WIDTH / 2, 150);
        }

        starfield = make_unique<Starfield>(250);
        player = make_unique<Spaceship>();

        // Setup intro texts
        introTexts = {
            "In the year 2387...",
            "Humanity spread across the galaxy,",
            "seeking new worlds to colonize.",
            "But something ancient awakened...",
            "EMPEROR DESTRUCTON",
            "A being of pure cosmic destruction.",
            "Now only one pilot stands in his way...",
            "YOU."
        };

        loadHighScores();
    }

    void update() {
        deltaTime = gameClock.restart().asSeconds();
        deltaTime = min(deltaTime, 0.05f) * slowTimeMultiplier;

        // Update screen shake
        if (shakeTimer > 0) {
            shakeTimer -= deltaTime;
            shakeOffset = Vector2(
                RandomGenerator::range(-shakeIntensity, shakeIntensity),
                RandomGenerator::range(-shakeIntensity, shakeIntensity)
            );
            shakeIntensity *= 0.95f;
        }
        else {
            shakeOffset = Vector2(0, 0);
        }

        // Update slow time
        if (slowTimeTimer > 0) {
            slowTimeTimer -= deltaTime / slowTimeMultiplier;
            if (slowTimeTimer <= 0) slowTimeMultiplier = 1.0f;
        }

        switch (currentScreen) {
        case GameScreen::Intro:
            updateIntro();
            break;
        case GameScreen::Menu:
            starfield->update(deltaTime);
            break;
        case GameScreen::Gameplay:
            updateGameplay();
            break;
        case GameScreen::BossWarning:
            updateBossWarning();
            break;
        default:
            starfield->update(deltaTime);
            break;
        }
    }

    void updateIntro() {
        introTimer += deltaTime;

        // Change intro text every 2.5 seconds
        if (introTimer > 2.5f) {
            introTimer = 0;
            currentIntroText++;
            if (currentIntroText >= static_cast<int>(introTexts.size())) {
                currentScreen = GameScreen::Menu;
                SoundManager::getInstance().playMusic("assets/menu_music.wav");
            }
        }
    }

    void updateBossWarning() {
        phaseTimer -= deltaTime;
        starfield->update(deltaTime);

        if (phaseTimer <= 0) {
            currentScreen = GameScreen::Gameplay;
            isBossLevel = true;
            boss = make_unique<FinalBoss>();
            SoundManager::getInstance().playMusic("assets/boss_music.wav");
        }
    }

    void updateGameplay() {
        starfield->update(deltaTime);
        particles.update(deltaTime);

        // Update player
        player->update(deltaTime);

        // Update enemies or boss
        if (isBossLevel && boss) {
            boss->setPlayerPosition(player->getPosition());
            boss->update(deltaTime);

            // Get boss bullets
            auto bossBullets = boss->getAttackBullets();
            for (auto& b : bossBullets) {
                bullets.push_back(move(b));
            }

            // Check if boss is defeated
            if (!boss->isActive()) {
                currentScreen = GameScreen::Victory;
                SoundManager::getInstance().playSound("victory");
                triggerScreenShake(20.0f, 1.0f);
            }
        }
        else {
            // Update enemies
            for (auto& enemy : enemies) {
                enemy->setPlayerPosition(player->getPosition());
                enemy->update(deltaTime);

                // Enemy firing
                if (enemy->canFire() && RandomGenerator::range(0, 100) < 3) {
                    fireEnemyBullet(enemy.get());
                    enemy->resetFireTimer();
                }
            }

            // Check phase completion
            if (enemies.empty() && phaseTimer <= 0) {
                nextPhase();
            }
        }

        // Update bullets
        for (auto& bullet : bullets) {
            bullet->update(deltaTime);
        }

        // Update power-ups
        for (auto& powerUp : powerUps) {
            powerUp->update(deltaTime);
        }

        // Update explosions
        for (auto& explosion : explosions) {
            explosion->update(deltaTime);
        }

        // Collision detection
        checkCollisions();

        // Remove inactive objects
        removeInactiveObjects();

        // Random power-up spawn
        if (RandomGenerator::range(0, 1000) < 2) {
            spawnPowerUp();
        }

        // Check game over
        if (player->getLives() <= 0 && player->getHealth() <= 0) {
            currentScreen = GameScreen::GameOver;
            SoundManager::getInstance().playSound("game_over");
        }

        phaseTimer -= deltaTime;
    }

    void firePlayerBullets() {
        if (!player->canFire()) return;
        player->resetFireTimer();
        SoundManager::getInstance().playSound("shoot");

        int shots = player->getMultiShotLevel();
        int power = player->getPowerLevel();

        if (shots == 1) {
            auto bullet = make_unique<Bullet>(true, 10 + power * 5);
            bullet->setPosition(player->getPosition() + Vector2(0, -30));
            bullet->setVelocity(Vector2(0, -600));
            bullets.push_back(move(bullet));
        }
        else if (shots == 2) {
            for (int i = -1; i <= 1; i += 2) {
                auto bullet = make_unique<Bullet>(true, 10 + power * 4);
                bullet->setPosition(player->getPosition() + Vector2(i * 15, -25));
                bullet->setVelocity(Vector2(0, -600));
                bullets.push_back(move(bullet));
            }
        }
        else if (shots >= 3) {
            auto center = make_unique<Bullet>(true, 10 + power * 5);
            center->setPosition(player->getPosition() + Vector2(0, -30));
            center->setVelocity(Vector2(0, -600));
            bullets.push_back(move(center));

            for (int i = -1; i <= 1; i += 2) {
                auto side = make_unique<Bullet>(true, 8 + power * 3);
                side->setPosition(player->getPosition() + Vector2(i * 20, -20));
                side->setVelocity(Vector2(i * 100, -550));
                bullets.push_back(move(side));
            }

            if (shots >= 4) {
                for (int i = -1; i <= 1; i += 2) {
                    auto wide = make_unique<Bullet>(true, 6 + power * 2);
                    wide->setPosition(player->getPosition() + Vector2(i * 25, -15));
                    wide->setVelocity(Vector2(i * 200, -500));
                    bullets.push_back(move(wide));
                }
            }
        }
    }

    void fireEnemyBullet(Enemy* enemy) {
        Vector2 dir = (player->getPosition() - enemy->getPosition()).normalized();
        auto bullet = make_unique<Bullet>(false, 10);
        bullet->setPosition(enemy->getPosition() + Vector2(0, 20));
        bullet->setVelocity(dir * 250.0f);
        bullets.push_back(move(bullet));
    }

    void checkCollisions() {
        // Player bullets vs enemies
        for (auto& bullet : bullets) {
            if (!bullet->isActive() || !bullet->isFromPlayer()) continue;

            // Check boss
            if (isBossLevel && boss && boss->isActive()) {
                if (bullet->checkCollision(boss.get())) {
                    boss->takeDamage(bullet->getDamage());
                    bullet->setActive(false);
                    particles.emit(bullet->getPosition(), Vector2(0, 0), sf::Color::Yellow, 5, 0.3f);
                    triggerScreenShake(3.0f, 0.1f);
                }
            }

            // Check enemies
            for (auto& enemy : enemies) {
                if (!enemy->isActive()) continue;
                if (bullet->checkCollision(enemy.get())) {
                    enemy->takeDamage(bullet->getDamage());
                    bullet->setActive(false);
                    particles.emit(bullet->getPosition(), Vector2(0, 0), sf::Color::Yellow, 5, 0.3f);

                    if (!enemy->isActive()) {
                        player->addScore(enemy->getScoreValue());
                        createExplosion(enemy->getPosition());
                        SoundManager::getInstance().playSound("explosion");
                        triggerScreenShake(5.0f, 0.15f);

                        // Chance to drop power-up
                        if (RandomGenerator::range(0, 100) < 20) {
                            spawnPowerUpAt(enemy->getPosition());
                        }
                    }
                }
            }
        }

        // Enemy bullets vs player
        for (auto& bullet : bullets) {
            if (!bullet->isActive() || bullet->isFromPlayer()) continue;
            if (bullet->checkCollision(player.get())) {
                player->takeDamage(bullet->getDamage() * difficulty);
                bullet->setActive(false);
                particles.emit(player->getPosition(), Vector2(0, 0), sf::Color::Red, 8, 0.4f);
                triggerScreenShake(4.0f, 0.1f);
            }
        }

        // Player vs enemies (collision damage)
        for (auto& enemy : enemies) {
            if (!enemy->isActive()) continue;
            if (enemy->checkCollision(player.get())) {
                player->takeDamage(20 * difficulty);
                enemy->takeDamage(30);
                triggerScreenShake(8.0f, 0.2f);
            }
        }

        // Player vs power-ups
        for (auto& powerUp : powerUps) {
            if (!powerUp->isActive()) continue;
            if (powerUp->checkCollision(player.get())) {
                PowerUpType type = powerUp->getType();

                if (type == PowerUpType::Nuke) {
                    // Destroy all enemies
                    for (auto& enemy : enemies) {
                        if (enemy->isActive()) {
                            player->addScore(enemy->getScoreValue() / 2);
                            createExplosion(enemy->getPosition());
                            enemy->setActive(false);
                        }
                    }
                    triggerScreenShake(15.0f, 0.5f);
                    SoundManager::getInstance().playSound("explosion");
                }
                else if (type == PowerUpType::Slow) {
                    slowTimeMultiplier = 0.4f;
                    slowTimeTimer = 5.0f;
                }
                else {
                    player->applyPowerUp(type);
                }

                powerUp->setActive(false);
                particles.emit(player->getPosition(), Vector2(0, 0), sf::Color::Cyan, 15, 0.5f, 4.0f);
            }
        }

        // Player vs boss collision
        if (isBossLevel && boss && boss->isActive() && boss->checkCollision(player.get())) {
            player->takeDamage(30 * difficulty);
            triggerScreenShake(12.0f, 0.3f);
        }
    }

    void createExplosion(const Vector2& pos, float scale = 1.0f) {
        auto explosion = make_unique<Explosion>(pos, scale);
        explosions.push_back(move(explosion));
        particles.emitExplosion(pos, 15, 4.0f);
    }

    void spawnPowerUp() {
        PowerUpType type = static_cast<PowerUpType>(RandomGenerator::range(0, 7));
        auto powerUp = make_unique<PowerUp>(type);
        powerUp->setPosition(Vector2(RandomGenerator::range(50.0f, SCREEN_WIDTH - 50.0f), -30));
        powerUps.push_back(move(powerUp));
    }

    void spawnPowerUpAt(const Vector2& pos) {
        PowerUpType type = static_cast<PowerUpType>(RandomGenerator::range(0, 7));
        auto powerUp = make_unique<PowerUp>(type);
        powerUp->setPosition(pos);
        powerUps.push_back(move(powerUp));
    }

    void removeInactiveObjects() {
        enemies.erase(remove_if(enemies.begin(), enemies.end(),
            [](const unique_ptr<Enemy>& e) { return !e->isActive(); }), enemies.end());
        bullets.erase(remove_if(bullets.begin(), bullets.end(),
            [](const unique_ptr<Bullet>& b) { return !b->isActive(); }), bullets.end());
        powerUps.erase(remove_if(powerUps.begin(), powerUps.end(),
            [](const unique_ptr<PowerUp>& p) { return !p->isActive(); }), powerUps.end());
        explosions.erase(remove_if(explosions.begin(), explosions.end(),
            [](const unique_ptr<Explosion>& e) { return !e->isActive(); }), explosions.end());
    }

    void nextPhase() {
        currentPhase++;

        if (currentPhase > PHASES_PER_LEVEL) {
            currentPhase = 1;
            currentLevel++;

            if (currentLevel > MAX_LEVELS) {
                // Transition to boss level
                currentScreen = GameScreen::BossWarning;
                phaseTimer = 4.0f;
                return;
            }

            SoundManager::getInstance().playSound("level_up");
        }

        phaseTimer = 25.0f + currentLevel * 5;
        spawnEnemies();
    }

    void spawnEnemies() {
        enemies.clear();

        int baseCount = 6 + currentLevel * 3 + currentPhase * 2;
        int enemyCount = static_cast<int>(baseCount * difficulty);

        for (int i = 0; i < enemyCount; i++) {
            unique_ptr<Enemy> enemy;
            float x = 80 + (i % 8) * 130;
            float y = -50 - (i / 8) * 80;

            int randType = RandomGenerator::range(0, 100);

            if (currentLevel == 1) {
                if (currentPhase == 1) {
                    if (randType < 60) enemy = make_unique<AlphaEnemy>(currentLevel, currentPhase);
                    else enemy = make_unique<BetaEnemy>(currentLevel, currentPhase);
                }
                else {
                    if (randType < 40) enemy = make_unique<AlphaEnemy>(currentLevel, currentPhase);
                    else if (randType < 70) enemy = make_unique<BetaEnemy>(currentLevel, currentPhase);
                    else enemy = make_unique<GammaEnemy>(currentLevel, currentPhase);
                }
            }
            else {
                if (randType < 20) enemy = make_unique<AlphaEnemy>(currentLevel, currentPhase);
                else if (randType < 40) enemy = make_unique<BetaEnemy>(currentLevel, currentPhase);
                else if (randType < 55) enemy = make_unique<GammaEnemy>(currentLevel, currentPhase);
                else if (randType < 70) enemy = make_unique<MonsterEnemy>(currentLevel, currentPhase);
                else if (randType < 85) enemy = make_unique<PhantomEnemy>(currentLevel, currentPhase);
                else enemy = make_unique<DragonEnemy>(currentLevel, currentPhase);
            }

            enemy->setPosition(Vector2(x, y));
            enemies.push_back(move(enemy));
        }
    }

    void triggerScreenShake(float intensity, float duration) {
        shakeIntensity = intensity;
        shakeTimer = duration;
    }

    // ========== DRAWING FUNCTIONS ==========

    void draw(sf::RenderWindow& window) {
        window.clear(sf::Color(5, 5, 15));

        sf::View view = window.getDefaultView();
        view.move(shakeOffset.x, shakeOffset.y);
        window.setView(view);

        switch (currentScreen) {
        case GameScreen::Intro: drawIntro(window); break;
        case GameScreen::Menu: drawMenu(window); break;
        case GameScreen::Instructions: drawInstructions(window); break;
        case GameScreen::Gameplay: drawGameplay(window); break;
        case GameScreen::Pause: drawPause(window); break;
        case GameScreen::HighScore: drawHighScores(window); break;
        case GameScreen::GameOver: drawGameOver(window); break;
        case GameScreen::Victory: drawVictory(window); break;
        case GameScreen::BossWarning: drawBossWarning(window); break;
        }

        window.setView(window.getDefaultView());
        window.display();
    }

    void drawIntro(sf::RenderWindow& window) {
        // Draw intro image/video frame
        window.draw(introSprite);

        // Fade overlay
        sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        if (!fontLoaded) return;

        // Draw current intro text
        if (currentIntroText < static_cast<int>(introTexts.size())) {
            sf::Text text;
            text.setFont(gameFont);
            text.setString(introTexts[currentIntroText]);
            text.setCharacterSize(36);
            text.setFillColor(sf::Color::White);

            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.width / 2, bounds.height / 2);
            text.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

            // Fade effect
            float alpha = min(introTimer / 0.5f, 1.0f) * 255;
            if (introTimer > 2.0f) alpha = (2.5f - introTimer) / 0.5f * 255;
            text.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));

            window.draw(text);
        }

        // Skip hint
        sf::Text skipText;
        skipText.setFont(gameFont);
        skipText.setString("Press SPACE or ENTER to skip");
        skipText.setCharacterSize(18);
        skipText.setFillColor(sf::Color(150, 150, 150));
        skipText.setPosition(SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT - 50);
        window.draw(skipText);

        // Progress bar
        float progress = static_cast<float>(currentIntroText) / introTexts.size();
        sf::RectangleShape progressBg(sf::Vector2f(400, 4));
        progressBg.setPosition(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 80);
        progressBg.setFillColor(sf::Color(50, 50, 50));
        window.draw(progressBg);

        sf::RectangleShape progressBar(sf::Vector2f(400 * progress, 4));
        progressBar.setPosition(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 80);
        progressBar.setFillColor(sf::Color(100, 150, 255));
        window.draw(progressBar);
    }

    void drawMenu(sf::RenderWindow& window) {
        window.draw(menuBackground);
        starfield->draw(window);

        if (!fontLoaded) return;

        // Logo
        window.draw(logoSprite);

        // Menu options
        vector<pair<string, string>> menuItems = {
            {"ENTER", "Start Game"},
            {"I", "Instructions"},
            {"H", "High Scores"},
            {"S", "Toggle Sound: " + string(soundEnabled ? "ON" : "OFF")},
            {"ESC", "Exit Game"}
        };

        float startY = 300;
        for (size_t i = 0; i < menuItems.size(); i++) {
            // Key box
            sf::RectangleShape keyBox(sf::Vector2f(60, 40));
            keyBox.setPosition(SCREEN_WIDTH / 2 - 180, startY + i * 60);
            keyBox.setFillColor(sf::Color(30, 30, 60));
            keyBox.setOutlineColor(sf::Color(100, 150, 255));
            keyBox.setOutlineThickness(2);
            window.draw(keyBox);

            sf::Text keyText;
            keyText.setFont(gameFont);
            keyText.setString(menuItems[i].first);
            keyText.setCharacterSize(16);
            keyText.setFillColor(sf::Color(100, 200, 255));
            keyText.setPosition(SCREEN_WIDTH / 2 - 170, startY + i * 60 + 10);
            window.draw(keyText);

            sf::Text optionText;
            optionText.setFont(gameFont);
            optionText.setString(menuItems[i].second);
            optionText.setCharacterSize(24);
            optionText.setFillColor(sf::Color::White);
            optionText.setPosition(SCREEN_WIDTH / 2 - 100, startY + i * 60 + 8);
            window.draw(optionText);
        }

        // Difficulty selector
        sf::Text diffText;
        diffText.setFont(gameFont);
        string diffStr = difficulty < 1.0f ? "EASY" : (difficulty < 1.3f ? "NORMAL" : "HARD");
        diffText.setString("Difficulty: " + diffStr + " (D to change)");
        diffText.setCharacterSize(18);
        diffText.setFillColor(sf::Color(200, 200, 100));
        diffText.setPosition(SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT - 80);
        window.draw(diffText);
    }

    void drawInstructions(sf::RenderWindow& window) {
        window.draw(menuBackground);
        starfield->draw(window);

        if (!fontLoaded) return;

        sf::Text title;
        title.setFont(gameFont);
        title.setString("INSTRUCTIONS");
        title.setCharacterSize(48);
        title.setFillColor(sf::Color(100, 200, 255));
        title.setPosition(SCREEN_WIDTH / 2 - 180, 40);
        window.draw(title);

        vector<string> instructions = {
            "CONTROLS:",
            "  Arrow Keys / WASD - Move spaceship",
            "  SPACE - Fire weapons",
            "  P - Pause game",
            "  M - Toggle sound",
            "  ESC - Return to menu",
            "",
            "POWER-UPS:",
            "  Yellow - Increase weapon power",
            "  Orange - Faster fire rate",
            "  Blue - Shield protection",
            "  Green - Extra life + heal",
            "  Purple - Multi-shot upgrade",
            "  Red - Temporary invincibility",
            "  Dark Purple - Screen nuke",
            "  Cyan - Slow time",
            "",
            "OBJECTIVE:",
            "  Survive 2 levels with 2 phases each",
            "  Then defeat EMPEROR DESTRUCTON!",
            "",
            "Press ESC to return"
        };

        float y = 110;
        for (const auto& line : instructions) {
            sf::Text text;
            text.setFont(gameFont);
            text.setString(line);
            text.setCharacterSize(18);
            text.setFillColor(line.find(':') != string::npos ? sf::Color(255, 200, 100) : sf::Color::White);
            text.setPosition(100, y);
            window.draw(text);
            y += 25;
        }
    }

    void drawGameplay(sf::RenderWindow& window) {
        // Draw appropriate background
        if (isBossLevel) {
            window.draw(bossBackground);
        }
        else {
            window.draw(gameBackground);
        }

        starfield->draw(window);
        particles.draw(window);

        // Draw power-ups
        for (auto& powerUp : powerUps) {
            powerUp->draw(window);
        }

        // Draw bullets
        for (auto& bullet : bullets) {
            bullet->draw(window);
        }

        // Draw enemies
        for (auto& enemy : enemies) {
            enemy->draw(window);
        }

        // Draw boss
        if (isBossLevel && boss) {
            boss->draw(window);
            boss->drawBossHealthBar(window, gameFont);
        }

        // Draw explosions
        for (auto& explosion : explosions) {
            explosion->draw(window);
        }

        // Draw player
        player->draw(window);

        // Draw HUD
        if (fontLoaded) {
            player->drawHUD(window, gameFont);

            // Level/Phase indicator
            if (!isBossLevel) {
                sf::Text levelText;
                levelText.setFont(gameFont);
                levelText.setString("Level " + to_string(currentLevel) + " - Phase " + to_string(currentPhase) + "/" + to_string(PHASES_PER_LEVEL));
                levelText.setCharacterSize(20);
                levelText.setFillColor(sf::Color(150, 200, 255));
                levelText.setPosition(SCREEN_WIDTH - 250, 20);
                window.draw(levelText);

                sf::Text enemyText;
                enemyText.setFont(gameFont);
                enemyText.setString("Enemies: " + to_string(enemies.size()));
                enemyText.setCharacterSize(16);
                enemyText.setFillColor(sf::Color(200, 150, 150));
                enemyText.setPosition(SCREEN_WIDTH - 250, 50);
                window.draw(enemyText);
            }
            else {
                sf::Text bossText;
                bossText.setFont(gameFont);
                bossText.setString("=== BOSS BATTLE ===");
                bossText.setCharacterSize(20);
                bossText.setFillColor(sf::Color(255, 100, 100));
                bossText.setPosition(SCREEN_WIDTH - 220, 60);
                window.draw(bossText);
            }

            // Slow time indicator
            if (slowTimeTimer > 0) {
                sf::Text slowText;
                slowText.setFont(gameFont);
                slowText.setString("SLOW TIME: " + to_string(static_cast<int>(slowTimeTimer)) + "s");
                slowText.setCharacterSize(24);
                slowText.setFillColor(sf::Color(100, 255, 255));
                slowText.setPosition(SCREEN_WIDTH / 2 - 80, 100);
                window.draw(slowText);
            }
        }
    }

    void drawPause(sf::RenderWindow& window) {
        drawGameplay(window);

        sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        if (!fontLoaded) return;

        sf::Text pauseText;
        pauseText.setFont(gameFont);
        pauseText.setString("PAUSED");
        pauseText.setCharacterSize(64);
        pauseText.setFillColor(sf::Color(100, 200, 255));
        pauseText.setPosition(SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 - 100);
        window.draw(pauseText);

        sf::Text resumeText;
        resumeText.setFont(gameFont);
        resumeText.setString("Press P to Resume\nPress ESC for Menu");
        resumeText.setCharacterSize(24);
        resumeText.setFillColor(sf::Color::White);
        resumeText.setPosition(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2);
        window.draw(resumeText);
    }

    void drawHighScores(sf::RenderWindow& window) {
        window.draw(menuBackground);
        starfield->draw(window);

        if (!fontLoaded) return;

        sf::Text title;
        title.setFont(gameFont);
        title.setString("HIGH SCORES");
        title.setCharacterSize(48);
        title.setFillColor(sf::Color(255, 200, 100));
        title.setPosition(SCREEN_WIDTH / 2 - 180, 50);
        window.draw(title);

        for (size_t i = 0; i < highScores.size() && i < 10; i++) {
            sf::Text entry;
            entry.setFont(gameFont);
            entry.setString(to_string(i + 1) + ". " + highScores[i].first + " - " + to_string(highScores[i].second));
            entry.setCharacterSize(24);
            entry.setFillColor(i < 3 ? sf::Color(255, 200, 0) : sf::Color::White);
            entry.setPosition(SCREEN_WIDTH / 2 - 200, 140 + i * 45);
            window.draw(entry);
        }

        sf::Text backText;
        backText.setFont(gameFont);
        backText.setString("Press ESC to return");
        backText.setCharacterSize(20);
        backText.setFillColor(sf::Color(150, 150, 200));
        backText.setPosition(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT - 60);
        window.draw(backText);
    }

    void drawGameOver(sf::RenderWindow& window) {
        starfield->draw(window);

        sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(50, 0, 0, 200));
        window.draw(overlay);

        if (!fontLoaded) return;

        sf::Text gameOverText;
        gameOverText.setFont(gameFont);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(72);
        gameOverText.setFillColor(sf::Color(255, 50, 50));
        gameOverText.setPosition(SCREEN_WIDTH / 2 - 250, 120);
        window.draw(gameOverText);

        sf::Text scoreText;
        scoreText.setFont(gameFont);
        scoreText.setString("Final Score: " + to_string(player->getScore()));
        scoreText.setCharacterSize(36);
        scoreText.setFillColor(sf::Color(255, 200, 100));
        scoreText.setPosition(SCREEN_WIDTH / 2 - 180, 240);
        window.draw(scoreText);

        sf::Text namePrompt;
        namePrompt.setFont(gameFont);
        namePrompt.setString("Enter your name:");
        namePrompt.setCharacterSize(24);
        namePrompt.setFillColor(sf::Color::White);
        namePrompt.setPosition(SCREEN_WIDTH / 2 - 120, 340);
        window.draw(namePrompt);

        sf::Text nameText;
        nameText.setFont(gameFont);
        nameText.setString(playerName + "_");
        nameText.setCharacterSize(28);
        nameText.setFillColor(sf::Color(100, 255, 100));
        nameText.setPosition(SCREEN_WIDTH / 2 - 100, 390);
        window.draw(nameText);

        sf::Text submitText;
        submitText.setFont(gameFont);
        submitText.setString("Press ENTER to submit");
        submitText.setCharacterSize(18);
        submitText.setFillColor(sf::Color(150, 150, 200));
        submitText.setPosition(SCREEN_WIDTH / 2 - 120, 460);
        window.draw(submitText);
    }

    void drawVictory(sf::RenderWindow& window) {
        starfield->draw(window);
        particles.draw(window);

        // Celebration particles
        if (RandomGenerator::range(0, 10) < 3) {
            particles.emit(Vector2(RandomGenerator::range(100.0f, SCREEN_WIDTH - 100.0f), SCREEN_HEIGHT + 20),
                Vector2(RandomGenerator::range(-50.0f, 50.0f), -300),
                sf::Color(RandomGenerator::range(100, 255), RandomGenerator::range(100, 255), RandomGenerator::range(100, 255)),
                5, 2.0f, 5.0f);
        }

        sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(0, 30, 0, 150));
        window.draw(overlay);

        if (!fontLoaded) return;

        sf::Text victoryText;
        victoryText.setFont(gameFont);
        victoryText.setString("VICTORY!");
        victoryText.setCharacterSize(80);
        victoryText.setFillColor(sf::Color(100, 255, 100));
        victoryText.setPosition(SCREEN_WIDTH / 2 - 200, 100);
        window.draw(victoryText);

        sf::Text defeatText;
        defeatText.setFont(gameFont);
        defeatText.setString("Emperor Destructon has been defeated!");
        defeatText.setCharacterSize(28);
        defeatText.setFillColor(sf::Color(200, 255, 200));
        defeatText.setPosition(SCREEN_WIDTH / 2 - 260, 220);
        window.draw(defeatText);

        sf::Text scoreText;
        scoreText.setFont(gameFont);
        scoreText.setString("Final Score: " + to_string(player->getScore()));
        scoreText.setCharacterSize(36);
        scoreText.setFillColor(sf::Color(255, 220, 100));
        scoreText.setPosition(SCREEN_WIDTH / 2 - 180, 300);
        window.draw(scoreText);

        sf::Text namePrompt;
        namePrompt.setFont(gameFont);
        namePrompt.setString("Enter your name for the Hall of Fame:");
        namePrompt.setCharacterSize(22);
        namePrompt.setFillColor(sf::Color::White);
        namePrompt.setPosition(SCREEN_WIDTH / 2 - 200, 400);
        window.draw(namePrompt);

        sf::Text nameText;
        nameText.setFont(gameFont);
        nameText.setString(playerName + "_");
        nameText.setCharacterSize(28);
        nameText.setFillColor(sf::Color(100, 255, 100));
        nameText.setPosition(SCREEN_WIDTH / 2 - 100, 450);
        window.draw(nameText);

        sf::Text submitText;
        submitText.setFont(gameFont);
        submitText.setString("Press ENTER to submit");
        submitText.setCharacterSize(18);
        submitText.setFillColor(sf::Color(150, 200, 150));
        submitText.setPosition(SCREEN_WIDTH / 2 - 120, 520);
        window.draw(submitText);
    }

    void drawBossWarning(sf::RenderWindow& window) {
        window.draw(bossBackground);
        starfield->draw(window);

        // Flashing red overlay
        float flash = sin(phaseTimer * 8) * 0.5f + 0.5f;
        sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(100, 0, 0, static_cast<sf::Uint8>(flash * 150)));
        window.draw(overlay);

        if (!fontLoaded) return;

        sf::Text warningText;
        warningText.setFont(gameFont);
        warningText.setString("! WARNING !");
        warningText.setCharacterSize(64);
        warningText.setFillColor(sf::Color(255, static_cast<sf::Uint8>(flash * 255), 0));
        warningText.setPosition(SCREEN_WIDTH / 2 - 200, 150);
        window.draw(warningText);

        sf::Text bossText;
        bossText.setFont(gameFont);
        bossText.setString("EMPEROR DESTRUCTON APPROACHES");
        bossText.setCharacterSize(36);
        bossText.setFillColor(sf::Color(255, 100, 100));
        bossText.setPosition(SCREEN_WIDTH / 2 - 300, 280);
        window.draw(bossText);

        sf::Text prepareText;
        prepareText.setFont(gameFont);
        prepareText.setString("Prepare for battle in: " + to_string(static_cast<int>(phaseTimer) + 1));
        prepareText.setCharacterSize(28);
        prepareText.setFillColor(sf::Color::White);
        prepareText.setPosition(SCREEN_WIDTH / 2 - 180, 400);
        window.draw(prepareText);
    }

    // ========== INPUT HANDLING ==========

    void handleEvent(sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            switch (currentScreen) {
            case GameScreen::Intro:
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return) {
                    currentScreen = GameScreen::Menu;
                    SoundManager::getInstance().playMusic("assets/menu_music.wav");
                }
                break;

            case GameScreen::Menu:
                if (event.key.code == sf::Keyboard::Return) {
                    startGame();
                }
                else if (event.key.code == sf::Keyboard::I) {
                    currentScreen = GameScreen::Instructions;
                }
                else if (event.key.code == sf::Keyboard::H) {
                    currentScreen = GameScreen::HighScore;
                }
                else if (event.key.code == sf::Keyboard::S) {
                    soundEnabled = !soundEnabled;
                    SoundManager::getInstance().toggleSound();
                }
                else if (event.key.code == sf::Keyboard::D) {
                    if (difficulty < 1.0f) difficulty = 1.0f;
                    else if (difficulty < 1.3f) difficulty = 1.5f;
                    else difficulty = 0.7f;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                break;

            case GameScreen::Instructions:
            case GameScreen::HighScore:
                if (event.key.code == sf::Keyboard::Escape) {
                    currentScreen = GameScreen::Menu;
                }
                break;

            case GameScreen::Gameplay:
                if (event.key.code == sf::Keyboard::P) {
                    currentScreen = GameScreen::Pause;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    currentScreen = GameScreen::Menu;
                }
                break;

            case GameScreen::Pause:
                if (event.key.code == sf::Keyboard::P) {
                    currentScreen = GameScreen::Gameplay;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    currentScreen = GameScreen::Menu;
                }
                break;

            case GameScreen::GameOver:
            case GameScreen::Victory:
                if (event.key.code == sf::Keyboard::Return) {
                    if (!playerName.empty()) {
                        addHighScore(playerName, player->getScore());
                        playerName.clear();
                        currentScreen = GameScreen::HighScore;
                    }
                }
                break;

            default:
                break;
            }
        }

        // Text input for name entry
        if (event.type == sf::Event::TextEntered) {
            if (currentScreen == GameScreen::GameOver || currentScreen == GameScreen::Victory) {
                if (event.text.unicode == 8 && !playerName.empty()) {
                    playerName.pop_back();
                }
                else if (event.text.unicode >= 32 && event.text.unicode < 128 && playerName.length() < 12) {
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
        }
    }

    void handleContinuousInput() {
        if (currentScreen != GameScreen::Gameplay) return;

        Vector2 velocity(0, 0);
        float speed = 300.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            velocity.y = -speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            velocity.y = speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            velocity.x = -speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            velocity.x = speed;
        }

        player->setVelocity(velocity);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            firePlayerBullets();
        }

        // Mute toggle
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            if (!mKeyPressed) {
                soundEnabled = !soundEnabled;
                SoundManager::getInstance().toggleSound();
                mKeyPressed = true;
            }
        }
        else {
            mKeyPressed = false;
        }
    }

    void startGame() {
        currentScreen = GameScreen::Gameplay;
        currentLevel = 1;
        currentPhase = 1;
        phaseTimer = 30.0f;
        isBossLevel = false;
        slowTimeMultiplier = 1.0f;
        slowTimeTimer = 0;

        enemies.clear();
        bullets.clear();
        powerUps.clear();
        explosions.clear();
        particles.clear();
        boss.reset();

        player->reset();
        spawnEnemies();

        SoundManager::getInstance().playMusic("assets/game_music.wav");
    }

    // ========== HIGH SCORE MANAGEMENT ==========

    void loadHighScores() {
        highScores.clear();
        ifstream file("highscores.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t pos = line.find_last_of(' ');
                if (pos != string::npos) {
                    string name = line.substr(0, pos);
                    int score = stoi(line.substr(pos + 1));
                    highScores.push_back({ name, score });
                }
            }
            file.close();
        }
        sort(highScores.begin(), highScores.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
    }

    void saveHighScores() {
        ofstream file("highscores.txt");
        if (file.is_open()) {
            for (const auto& score : highScores) {
                file << score.first << " " << score.second << endl;
            }
            file.close();
        }
    }

    void addHighScore(const string& name, int score) {
        highScores.push_back({ name, score });
        sort(highScores.begin(), highScores.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        if (highScores.size() > static_cast<size_t>(MAX_HIGH_SCORES)) {
            highScores.resize(MAX_HIGH_SCORES);
        }
        saveHighScores();
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  SPACE SHOOTER ULTIMATE EDITION v5.0  " << endl;
    cout << "========================================" << endl;
    cout << "Starting game..." << endl;

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(SCREEN_WIDTH),
        static_cast<unsigned int>(SCREEN_HEIGHT)),
        GAME_TITLE);
    window.setFramerateLimit(TARGET_FPS);

    cout << "Window created: " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << endl;

    GameState game;

    cout << "Game initialized. Starting main loop..." << endl;
    cout << "========================================" << endl;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            game.handleEvent(event, window);
        }

        game.handleContinuousInput();
        game.update();
        game.draw(window);
    }

    cout << "Game closed. Thank you for playing!" << endl;
    return 0;
}
