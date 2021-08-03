#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <stack>
#include <utility>
#include <queue>
#include <deque>

class Visualiser{
private:
	std::vector<sf::RectangleShape> bars;
	std::vector<sf::RectangleShape> backup;

	sf::Vector2f start, end;
	float width, gap;

	float target, counter, speed;
	float timeTaken;
	bool isWorking;

	std::vector<sf::RectangleShape*> barsToResetColor;
	void setActiveBars(sf::RectangleShape* w, sf::RectangleShape* g, sf::RectangleShape* b = nullptr);

	std::function<void()> algo;

	struct {
		int outer = 0, inner = 0;
	} bubble;
	struct {
		int outer, inner, min;
	} selection;
	struct {
		int outer, inner, index;
		sf::RectangleShape bar;
	} insertion;
	struct {
		int outer, inner;
		sf::RectangleShape* pivot;
		std::stack<std::pair<int, int>> stack;
	} quick_ll;
	struct {
		int left, right;
		sf::RectangleShape* pivot;
		std::stack<std::pair<int, int>> stack;
	} quick_lr;
	struct {
		int first, second, merged;
		std::vector<sf::RectangleShape> one, two;
		std::stack<std::pair<std::pair<int, int>, std::pair<int, int>>> stack;
	} merge;
	struct {
		int first, second, merged;
		std::vector<sf::RectangleShape> one, two;
		std::stack<std::pair<std::pair<int, int>, std::pair<int, int>>> stack;
	} merge2;

	void bubble_sort();
	void selection_sort();
	void insertion_sort();
	void quick_sort_LL();
	void quick_sort_LR();
	void merge_sort();
	void merge_sort2();

public:
	Visualiser();
	~Visualiser();

	void create(const sf::Vector2f& winSize);
	void loadAgain();

	float getSpeedLimit();
	void setSpeed(float speed);
	float getWidthLimit();
	void setWidth(float width);
	
	void update(float dt);
	void draw(sf::RenderWindow& window);

	enum {NONE, BUBBLE, SELECTION, INSERTION, QUICK_LL, QUICK_LR, MERGE, MERGE_2};
	void setAlgorithm(int sort_algo);
};