#pragma once
template <typename T, typename U>
sf::Vector2f centerPosition(T& parent, U& child) {
    return sf::Vector2f(parent.getPosition().x / 2.0 - child.getSize().x / 2.0, parent.getPosition().y / 2.0 - child.getSize().y / 2.0);
}
template <typename U>
sf::Vector2f centerPosition(sf::RenderWindow& window, U& child) {
    return sf::Vector2f(window.getSize().x / 2.0 - child.getSize().x / 2.0, window.getSize().y / 2.0 - child.getSize().y / 2.0);
}
template <typename T>
sf::Vector2f centerPosition(T& parent, sf::Text& text) {
    return sf::Vector2f(parent.getPosition().x + ((parent.getSize().x - text.getGlobalBounds().width) / 2.0), parent.getPosition().y + ((parent.getSize().y - text.getGlobalBounds().height) / 2.0));
}
