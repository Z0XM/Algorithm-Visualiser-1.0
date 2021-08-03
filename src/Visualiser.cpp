#include "Visualiser.hpp"
#include "GUI.hpp"
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

Visualiser::Visualiser()
{
	width = 2;
}

Visualiser::~Visualiser()
{
	
}

void Visualiser::create(const sf::Vector2f& winSize)
{
	bars.clear();

	start = { 50, 50 };
	end = { winSize.x - 100, winSize.y - 100 };
	gap = 1;

	bool center_placed = false;

	int N = (end.x - start.x) / (width + gap);
	for (int i = 0; i < N; i++)
	{
		sf::RectangleShape bar;
		bar.setFillColor(sf::Color::Red);
		bar.setSize({ width, (i + 1) * ((end.y - start.y) / N)});
		if (center_placed) {
			bar.setOrigin(bar.getSize().x * 0.5f, bar.getSize().y * 0.5f);
			bar.setPosition(i * (gap + width) + gap * 0.5f + start.x, start.y + (end.y - start.y) / 2);
		} else 
			bar.setPosition(i * (gap + width) + gap * 0.5f + start.x, end.y - bar.getSize().y);
		
		bars.push_back(bar);
	}

	std::shuffle(bars.begin(), bars.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

	backup = bars;

	barsToResetColor.clear();
	counter = 0;
	target = 2;
	speed = 1;
	algo = nullptr;
	timeTaken = 0;
	isWorking = false;

	gui::Page::Group["Page"]->Sliders["Speed"]->reset();
}

void Visualiser::loadAgain()
{
	bars = backup;

	barsToResetColor.clear();
	counter = 0;
	algo = nullptr;
	timeTaken = 0;
	isWorking = false;
}

float Visualiser::getSpeedLimit()
{
	return 100;
}

void Visualiser::setSpeed(float _speed)
{
	speed = _speed;
	speed = std::max(1.f, speed);
	speed = std::min(getSpeedLimit(), speed);

	if (speed == getSpeedLimit())target = 0;
	else target = 2;
}

float Visualiser::getWidthLimit()
{
	return 50;
}

void Visualiser::setWidth(float _w)
{
	width = _w;
	width = std::max(2.f, width);
	width = std::min(getSpeedLimit(), width);
}

std::string secondsAsTime(float t) {
	int ms = int(t * 100) % 100;
	int s = t;
	int m = s / 60;
	s = s % 60;

	return std::to_string(m) + ":" + std::to_string(s) + ":" + std::to_string(ms);
}

void Visualiser::update(float dt)
{
	counter += dt * speed;
	if (isWorking)timeTaken += dt;

	gui::Page::Group["Page"]->Textboxes["time"]->setString(secondsAsTime(timeTaken));

	if (counter >= target) {
		if (algo != nullptr) {
			for (int i = 0; i < barsToResetColor.size(); i++) {
				barsToResetColor[i]->setFillColor(sf::Color::Red);
			}
			barsToResetColor.clear();
			algo();
		}
		counter = 0;
	}
}

void Visualiser::draw(sf::RenderWindow& window)
{
	for(int i = 0; i < bars.size(); i++){
		bars[i].setPosition(i * (gap + width) + gap * 0.5f + start.x, bars[i].getPosition().y);
		window.draw(bars[i]);
	}
}

void Visualiser::setAlgorithm(int sort_algo)
{
	if (sort_algo != NONE) {
		timeTaken = 0;
		isWorking = true;
	}

	switch (sort_algo) {
	case BUBBLE:  
		bubble.outer = 0; 
		bubble.inner = 0; 
		algo = [this]() {bubble_sort(); }; 
		break;
	case SELECTION: 
		selection.outer = 0;
		selection.inner = 1;
		selection.min = 0;
		algo = [this]() {selection_sort(); }; 
		break;
	case INSERTION:
		insertion.outer = 1;
		insertion.inner = 0;
		insertion.bar = bars[1];
		algo = [this]() {insertion_sort(); };
		break;
	case QUICK_LL:
		quick_ll.outer = -1;
		quick_ll.inner = 0;
		quick_ll.pivot = &bars[bars.size() - 1];
		while (!quick_ll.stack.empty())quick_ll.stack.pop();
		quick_ll.stack.push(std::make_pair(0, bars.size() - 1));
		algo = [this]() {quick_sort_LL(); };
		break;
	case QUICK_LR:
		quick_lr.left = 1;
		quick_lr.right = bars.size() - 1;
		quick_lr.pivot = &bars[0];
		while (!quick_lr.stack.empty())quick_lr.stack.pop();
		quick_lr.stack.push(std::make_pair(0, bars.size() - 1));
		algo = [this]() {quick_sort_LR(); };
		break;
	case MERGE:
		merge.first = 0;
		merge.second = 0;
		merge.one.clear();
		merge.two.clear();
		while (!merge.stack.empty())merge.stack.pop();
		{
			std::vector<std::pair<int, int>> s;
			s.push_back(std::make_pair(0, bars.size()));
			int top = 0;
			while (top < s.size()) {
				if (s[top].first != (s[top].first + s[top].second) / 2) {
					s.insert(s.begin() + top + 1, std::make_pair(s[top].first, (s[top].first + s[top].second) / 2));
					s.insert(s.begin() + top + 1, std::make_pair((s[top].first + s[top].second) / 2, s[top].second));
					merge.stack.push(std::make_pair(
						std::make_pair(s[top].first, (s[top].first + s[top].second) / 2),
						std::make_pair((s[top].first + s[top].second) / 2, s[top].second)
					));
				}
				top++;
			}
			auto info = merge.stack.top();
			merge.one.insert(merge.one.begin(), bars.begin() + info.first.first, bars.begin() + info.first.second);
			merge.two.insert(merge.two.begin(), bars.begin() + info.second.first, bars.begin() + info.second.second);
			merge.merged = info.first.first;
		}
		algo = [this]() {merge_sort(); };
		break;
	case MERGE_2:
		merge2.first = 0;
		merge2.second = 0;
		merge2.one.clear();
		merge2.two.clear();
		while (!merge2.stack.empty())merge2.stack.pop();
		{
			std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> v;
			int top = -1;
			v.push_back(std::make_pair(std::make_pair(0, bars.size() / 2 + 1), std::make_pair(bars.size() / 2 + 1, bars.size())));
			while (top + 1 < v.size()) {
				auto k = v[++top];
				if (k.first.first != (k.first.second + k.first.first) / 2)
					v.push_back(std::make_pair(std::make_pair(k.first.first, (k.first.second + k.first.first) / 2), std::make_pair((k.first.second + k.first.first) / 2, k.first.second)));
				if (k.second.first != (k.second.second + k.second.first) / 2)
					v.push_back(std::make_pair(std::make_pair(k.second.first, (k.second.second + k.second.first) / 2), std::make_pair((k.second.second + k.second.first) / 2, k.second.second)));
			}
			for (auto k : v)merge2.stack.push(k);
			auto info = merge2.stack.top();
			merge2.one.insert(merge2.one.begin(), bars.begin() + info.first.first, bars.begin() + info.first.second);
			merge2.two.insert(merge2.two.begin(), bars.begin() + info.second.first, bars.begin() + info.second.second);
			merge2.merged = info.first.first;
		}
		
		algo = [this]() {merge_sort2(); };
		break;
	default:
		isWorking = false;
		algo = nullptr;
	}
}

void Visualiser::setActiveBars(sf::RectangleShape* w, sf::RectangleShape* g, sf::RectangleShape* b)
{
	if (w != nullptr) {
		w->setFillColor(sf::Color::White);
		barsToResetColor.push_back(w);
	}
	if (g != nullptr) {
		g->setFillColor(sf::Color::Green);
		barsToResetColor.push_back(g);
	}

	if (b != nullptr) {
		b->setFillColor(sf::Color::Blue);
		barsToResetColor.push_back(b);
	}
}

bool operator<(const sf::RectangleShape& r1, const sf::RectangleShape& r2)
{
	return r1.getSize().y < r2.getSize().y;
}
bool operator>(const sf::RectangleShape& r1, const sf::RectangleShape& r2)
{
	return r1.getSize().y > r2.getSize().y;
}
bool operator==(const sf::RectangleShape& r1, const sf::RectangleShape& r2)
{
	return r1.getSize().y == r2.getSize().y;
}

void Visualiser::bubble_sort()
{
	int& inner = bubble.inner;
	int& outer = bubble.outer;

	if (outer < bars.size() - 1) {
		if (inner < bars.size() - outer - 1) {
			if (bars[inner + 1] < bars[inner])
				std::swap(bars[inner], bars[inner + 1]);
		
			setActiveBars(&bars[bars.size() - outer - 1], &bars[inner + 1]);
			inner++;
		}
		else {
			outer++;
			inner = 0;
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::selection_sort()
{
	int& outer = selection.outer;
	int& inner = selection.inner;
	int& min = selection.min;

	if (outer < bars.size() - 1) {
		if (inner < bars.size()) {
			if (bars[inner] < bars[min])
				min = inner;

			setActiveBars(&bars[outer], &bars[inner], &bars[min]);
			inner++;
		}
		else {
			std::swap(bars[min], bars[outer]);
			outer++;
			inner = outer + 1;
			min = outer;
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::insertion_sort()
{
	int& outer = insertion.outer;
	int& inner = insertion.inner;
	int& index = insertion.index;
	sf::RectangleShape& bar = insertion.bar;
	
	if (outer < bars.size()) {
		if (inner >= 0 && bars[inner] > bar)
		{
			bars[inner + 1] = bars[inner];
			setActiveBars(&bars[outer], &bars[inner]);
			inner--;
		}
		else {
			bars[inner + 1] = bar;
			outer++;
			inner = outer - 1;
			if (outer < bars.size())bar = bars[outer];
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::quick_sort_LR()
{
	int& left = quick_lr.left;
	int& right = quick_lr.right;
	auto& stack = quick_lr.stack;

	if (!stack.empty()) {
		if (left < right) {
			bool ml = bars[left] < *quick_lr.pivot, mr = bars[right] > * quick_lr.pivot;
			if (ml)left++;
			if (mr)right--;
			if (!ml && !mr && left < right) {
				std::swap(bars[left], bars[right]);
			}
			setActiveBars(quick_lr.pivot, &bars[left], &bars[right]);
		}
		else {
			auto info = stack.top();
			stack.pop();
			if(*quick_lr.pivot > bars[right])std::swap(bars[right], *quick_lr.pivot);

			if (right - 1 > info.first)
				stack.push(std::make_pair(info.first, right - 1));
			if (info.second > right)
				stack.push(std::make_pair(right, info.second));

			if (!stack.empty()) {
				left = stack.top().first + 1;
				right = stack.top().second;
				quick_lr.pivot = &bars[stack.top().first];
			}
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::quick_sort_LL()
{
	int& outer = quick_ll.outer;
	int& inner = quick_ll.inner;
	auto& stack = quick_ll.stack;

	if (!stack.empty()) {
		if (inner < stack.top().second) {
			if (bars[inner] < *quick_ll.pivot) {
				outer++;
				std::swap(bars[outer], bars[inner]);
			}
			if (outer == -1)setActiveBars(quick_ll.pivot, &bars[inner], nullptr);
			else setActiveBars(quick_ll.pivot, &bars[inner], &bars[outer]);
			inner++;
		}
		else {
			outer++;
			auto info = stack.top();
			stack.pop();
			std::swap(bars[outer], bars[info.second]);

			if (outer - 1 > info.first)
				stack.push(std::make_pair(info.first, outer - 1));
			if (info.second > outer + 1)
				stack.push(std::make_pair(outer + 1, info.second));

			if (!stack.empty()) {
				outer = stack.top().first - 1;
				inner = stack.top().first;
				quick_ll.pivot = &bars[stack.top().second];
			}
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::merge_sort()
{
	auto& first = merge.first;
	auto& second = merge.second;
	auto& merged = merge.merged;
	auto& stack = merge.stack;
	auto& one = merge.one;
	auto& two = merge.two;

	if (!stack.empty()) {
		if(merged < bars.size())setActiveBars(&bars[merged], &bars[std::min(stack.top().first.first + first, stack.top().first.second - 1)], &bars[std::min(stack.top().second.first + second, stack.top().second.second - 1)]);
		if (first < one.size() && second < two.size()) {
			if (one[first] < two[second]) {
				bars[merged] = one[first];
				first++;
			}
			else {
				bars[merged] = two[second];
				second++;
			}
			merged++;
		}
		else if (first < one.size()) {
			bars[merged] = one[first];
			first++; merged++;
		}
		else if (second < two.size()) {
			bars[merged] = two[second];
			second++; merged++;
		}
		else {
			stack.pop();
			if (!stack.empty()) {
				one.clear(); two.clear();
				auto info = stack.top();

				if (info.first.second == 256) {
					int a = 1;
				}
				merge.one.insert(merge.one.begin(), bars.begin() + info.first.first, bars.begin() + info.first.second);
				merge.two.insert(merge.two.begin(), bars.begin() + info.second.first, bars.begin() + info.second.second);

				merged = info.first.first;
				first = second = 0;
			}
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::merge_sort2()
{
	auto& first = merge2.first;
	auto& second = merge2.second;
	auto& merged = merge2.merged;
	auto& stack = merge2.stack;
	auto& one = merge2.one;
	auto& two = merge2.two;

	if (!stack.empty()) {
		if (merged < bars.size())setActiveBars(&bars[merged], &bars[std::min(stack.top().first.first + first, stack.top().first.second - 1)], &bars[std::min(stack.top().second.first + second, stack.top().second.second - 1)]);
		if (first < one.size() && second < two.size()) {
			if (one[first] < two[second]) {
				bars[merged] = one[first];
				first++;
			}
			else {
				bars[merged] = two[second];
				second++;
			}
			merged++;
		}
		else if (first < one.size()) {
			bars[merged] = one[first];
			first++; merged++;
		}
		else if (second < two.size()) {
			bars[merged] = two[second];
			second++; merged++;
		}
		else {
			stack.pop();
			if (!stack.empty()) {
				one.clear(); two.clear();
				auto info = stack.top();

				if (info.first.second == 256) {
					int a = 1;
				}
				merge2.one.insert(merge2.one.begin(), bars.begin() + info.first.first, bars.begin() + info.first.second);
				merge2.two.insert(merge2.two.begin(), bars.begin() + info.second.first, bars.begin() + info.second.second);

				merged = info.first.first;
				first = second = 0;
			}
		}
	}
	else setAlgorithm(NONE);
}