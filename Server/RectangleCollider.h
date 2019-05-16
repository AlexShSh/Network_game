#include <SFML/Graphics/RectangleShape.hpp>

class RectangleCollider
{
public:
    RectangleCollider(sf::Vector2f& position, sf::Vector2f& size);
    RectangleCollider() = default;

    void set_position(sf::Vector2f& position);
    void set_size(sf::Vector2f size);
    sf::Vector2f get_posotion() const;
    sf::Vector2f get_size() const;

    bool detect_collision(const RectangleCollider& other) const;

private:
    sf::RectangleShape rectangle;
};
