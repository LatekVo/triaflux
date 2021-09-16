#include <cmath>
#include <math.h>
#include <vector>
const float PI = 3.14159; // for rad conversion

class Triangle {
public:
	float a = 0, b = 0, c = 0;

	float base_x = 1, base_y = 0;
	float tip_x = 0,  tip_y = 0; //to be set

	//results
	float bari_x = 0, bari_y = 0;
	float ortho_x = 0, ortho_y = 0;
	
	float flux = 0; //diff
	
	//A - left bot
	//B - right bot
	//C - found pt

	Triangle(float alpha, float beta, float theta) {
	
		float base_len = std::sqrt(std::pow(base_x, 2) + std::pow(base_y, 2));

		a = alpha / 180 * PI;
		b = beta  / 180 * PI;
		c = theta / 180 * PI;

		// lhs / base_len = sin a / sin b
		float lhs = base_len * (std::sin(a) / std::sin(b));
		
		// rotation matrix of C
		tip_x = lhs * std::cos(a); // y = 0
		tip_y = lhs * std::sin(a); // y = 0

		//verts = [0,0] [base_x, base_y] [x, y]
		bari_x = (tip_x + base_x) / 3.f;
		bari_y = (tip_y + base_y) / 3.f;

		//SLOPES section removed
	
		//note: i have no idea how this works, this is too hard to comperhand to me
		// x = (x1 * tan(α) + x2 * tan(β) + x3 * tan(γ)) / (tan(α) + tan(β) + tan(γ))
		// y = (y1 * tan(α) + y2 * tan(β) + y3 * tan(γ)) / (tan(α) + tan(β) + tan(γ))

		ortho_x = (0 * 0 + base_x * std::tan(b) + tip_x * std::tan(c)) / (std::tan(a) + std::tan(b) + std::tan(c));
		ortho_y = (0 * 0 + base_y * std::tan(b) + tip_y * std::tan(c)) / (std::tan(a) + std::tan(b) + std::tan(c));
	
		// pythagorean distance from baricenter to orthocenter
		flux = std::sqrt(std::pow(ortho_x - bari_x, 2) + std::pow(ortho_y - bari_y, 2));
		//std::cout << "calculated a triangle | d:" << alpha << " " << beta << " " << theta << "  | o: " << ortho_x << " " << ortho_y << " | c: " << tip_x << " " << tip_y << "\n";
		
	}
};

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Config.hpp>
#include <SFML/Window.hpp>

#include <iostream> // dbg
#include <string>

int main() {

	std::vector<std::vector<Triangle>> trigList;

	//using the AAS rule
	for (float a = 90; a < 179; a += 0.3f) {
		std::vector<Triangle> oneAng;
		for (float b = 1; b < 180 - a; b += 0.3f) {
			// a + b + c = 180
			float c = 180.f - a - b;
		
			//this method takes more memory than required, but ensures smooth animation
			oneAng.push_back(Triangle(a, b, c));

		}
		trigList.push_back(oneAng);
	}
	
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "My window", sf::Style::Resize | sf::Style::Close);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(180);

	sf::Font font;

	font.loadFromFile("swan.ttf");

	//screen divided in 4, graph, trig, off, stats, all the same size based on padding
	float box_padding = 0.05;
	float box_sectionSize_x; // moved to draw section
	float box_sectionSize_y;

	sf::Color color_main = sf::Color::White;
	sf::Color color_side(31, 145, 226);
	sf::Color color_back = sf::Color::Black;

	auto drawTrig = [&] (Triangle trig) {
		float x_off = 1;
		float y_off = 1;

		float x_total;
		float y_total;

		sf::VertexArray trigOutline(sf::LineStrip, 4);	
		
	};
	
	auto drawStats = [&] (Triangle trig) {
		float x_off = 1;
		float y_off = 0;
	
		float x_start = window.getSize().x / 2.f + window.getSize().x * box_padding;
		float x_end = x_start + box_sectionSize_x;
		float x_len = x_end - x_start;
		float x_center = box_sectionSize_x / 2.f + x_start;

		float y_start = window.getSize().y / 2.f + window.getSize().y * box_padding;
		float y_end = y_start + box_sectionSize_y;
		float y_len = y_end - y_start;
		float y_center = box_sectionSize_y / 2.f + y_start;
	};
	
	auto drawGraph = [&] (std::vector<Triangle> trigs, int trig_idx) {
		float x_off = 0;
		float y_off = 0;
		
		float x_start = window.getSize().x * box_padding;
		float x_end = x_start + box_sectionSize_x;
		float x_len = x_end - x_start;

		float y_start = window.getSize().y * box_padding;
		float y_end = y_start + box_sectionSize_y;
		float y_len = y_end - y_start;
		float y_center = box_sectionSize_y / 2.f + y_start;

		sf::VertexArray graph_horizontalLine(sf::LineStrip, 2);
		graph_horizontalLine[0].position = sf::Vector2f(x_start, y_center);	
		graph_horizontalLine[1].position = sf::Vector2f(x_end, y_center);	
		graph_horizontalLine[0].color = color_side;	
		graph_horizontalLine[1].color = color_side;	
		
		sf::VertexArray graph_verticalLine(sf::LineStrip, 2);
		graph_verticalLine[0].position = sf::Vector2f(x_start, y_start);	
		graph_verticalLine[1].position = sf::Vector2f(x_start, y_end);
		graph_verticalLine[0].color = color_side;
		graph_verticalLine[1].color = color_side;

		float max = 0; // 0 is min
		for (auto trig : trigs) {
			if (trig.flux > max)
				max = trig.flux;
		}
	
		sf::Text text;
		text.setFont(font); // FILL THIS OR SEGV 
		text.setString(std::to_string(max));
		text.setCharacterSize(12); 
		text.setFillColor(color_side);
		text.setPosition(x_start, y_start);
		
		// 0 > y_max | 0 > 1 | 0 > box_sectionSize_y, 
		float sizeMultiplier = box_sectionSize_y / max;
		float xMultiplier = box_sectionSize_x / trigs.size();
		
		float arrow_base = 0.01f;
		sf::VertexArray graph_arrow(sf::LineStrip, 4);
		graph_arrow[0].position = sf::Vector2f(x_start + trig_idx * xMultiplier, y_center + (window.getSize().y * 0.01f));
		graph_arrow[1].position = sf::Vector2f(x_start + trig_idx * xMultiplier - (window.getSize().x * arrow_base / 2), y_center + (window.getSize().y * (0.01f + arrow_base)));
		graph_arrow[2].position = sf::Vector2f(x_start + trig_idx * xMultiplier + (window.getSize().x * arrow_base / 2), y_center + (window.getSize().y * (0.01f + arrow_base)));
		graph_arrow[3].position = sf::Vector2f(x_start + trig_idx * xMultiplier, y_center + (window.getSize().y * 0.01f));
		
		sf::VertexArray lines(sf::LineStrip, trigs.size());
		
		//mind you, this displays every flux in line
		for (int i = 0; i < trigs.size(); i++) {
			lines[i].position = sf::Vector2f(x_start + i * xMultiplier, y_center + (-trigs[i].flux * sizeMultiplier / 2.f));	
		}

		window.draw(graph_arrow);
		window.draw(graph_horizontalLine);
		window.draw(graph_verticalLine);
		window.draw(lines);
		window.draw(text);

	};

	auto drawFlux = [&] (Triangle trig) {
	
		float baseUnit = (window.getSize().x + window.getSize().y) * 0.003f; //window size dependant unit
		
		float x_off = 0;
		float y_off = 1;
		
		float x_start = window.getSize().x * box_padding;
		float x_end = x_start + box_sectionSize_x;
		float x_len = x_end - x_start;
		float x_center = box_sectionSize_x / 2.f + x_start;
		
		float y_start = window.getSize().y / 2.f + window.getSize().y * box_padding;
		float y_end = y_start + box_sectionSize_y;
		float y_len = y_end - y_start;
		float y_center = box_sectionSize_y / 2.f + y_start;

		//note that this project is not meant to be practical, or to look good, but for me to learn, excuse this random use of trenary
		float biggerDim = window.getSize().x > window.getSize().y ? window.getSize().x : window.getSize().y;
		biggerDim /= 2;
		biggerDim -= biggerDim * box_padding;

		sf::VertexArray lines(sf::LineStrip, 2);	
		sf::CircleShape bari(baseUnit);
		sf::CircleShape ortho(baseUnit);

		float x = trig.ortho_x - trig.bari_x,
			  y = trig.ortho_y - trig.bari_y;

		float biggerVar = trig.ortho_x - trig.bari_x > trig.ortho_y - trig.bari_y ? x : y;

		x = x / biggerVar * biggerDim;
		y = y / biggerVar * biggerDim;

		bari.setPosition(x, y);
		bari.setFillColor(color_main);
		ortho.setPosition(x, y);
		ortho.setFillColor(color_side);
		
		lines[0].position = sf::Vector2f(x_center, y_center);
		lines[1].position = sf::Vector2f(x, y);

		window.draw(lines);
		window.draw(bari);
		window.draw(ortho);
	};

	while(1) {
		//draw loop
		for (auto trigClump : trigList) {
			
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					window.close();
					return 0;
				}
			}
			
			int trig_idx = 0;
			for (auto trig : trigClump) {
				
				window.clear(sf::Color::Black);
				
				//size adjustment
				box_sectionSize_x = (window.getSize().x / 2.f) - (window.getSize().x * box_padding); //padding l, r, 2x center
				box_sectionSize_y = (window.getSize().y / 2.f) - (window.getSize().y * box_padding); // /2 *2 (half the size, both side padding)
		
				drawTrig(trig);
				drawFlux(trig);
				drawStats(trig);
				drawGraph(trigClump, trig_idx);
				
				trig_idx++;
				
				window.display();
			}
			
		}
	}
	window.close();
	return 0;
}
