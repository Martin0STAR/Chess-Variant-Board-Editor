#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <filesystem>

namespace BoardComponent
{
	class ProfileBox : public sf::Drawable
	{
	public:
		ProfileBox();
		ProfileBox(
			std::string profilename,
			sf::Vector2f position,
			sf::Vector2u size,
			unsigned int border1width, unsigned int border2width,
			sf::Color border1color, sf::Color border2color);

		friend std::istream & operator >> (std::istream &is, ProfileBox& profilebox);
		friend std::ostream & operator << (std::ostream &os, const ProfileBox& profilebox);

		std::string getName() const;
		sf::Vector2f getPosition() const;

		sf::Vector2f getTopLeftPosition() const;
		sf::Vector2f getBottomRightPosition() const;
		float getLeftX() const;
		float getRightX() const;
		float getTopY() const;
		float getBottomY() const;
		sf::Vector2u getSize() const;
		unsigned int getBorderWidth() const;
		unsigned int getHighlightWidth() const;
		sf::Color getPlayerColor() const;
		sf::Color getHighlightColor() const;
		bool isHighlighted() const;
		bool isWithinBorder(int, int) const;
		bool isWithinBorder(float, float) const;
		bool set(std::string name, sf::Color bordercolor);
		bool setPlayerColor(sf::Color);
		bool setHighLightColor(sf::Color);
		bool setPosition(sf::Vector2f position);
		bool unhighlight();
		bool highlight();
		void drawProfilePicture(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawBorder(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawHighlightBorder(sf::RenderTarget& target, sf::RenderStates states) const;
		void move(const sf::Vector2f &);
		bool saveProfileImage();
		bool updateProfilePicture();
	private:

		std::string getImageFileName();
		std::string getNoAvatarFileName();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		std::string _profilename;
		sf::Vector2f _position;
		sf::Vector2u _size;
		unsigned int _border1width;
		unsigned int _border2width;
		sf::Color _border1color;
		sf::Color _border2color;
		bool _ishighlighted;
		sf::Image _profileimage;

		std::map<std::string, std::string> _urilist
		{
			{"Martin0", "/uploads/v1/user/1786183.123de062.200x200o.737497a59fcf.gif"},
			{"HolographWars", "/uploads/v1/user/35915414.c56a5656.161x161o.24331ae45a6a.jpeg"},
			{"thegreatauk", "/uploads/v1/user/12477896.52ba4eb6.161x161o.f4a08a5ff8bd.jpeg"},
			{"rychessmaster1", "/uploads/v1/user/19962328.457f17b1.161x161o.8e25ca6ba258.jpeg"},
			{"captaintugwash", "/uploads/v1/user/3114402.3aacc400.161x161o.74bc7e64452b.jpeg"},
			{"evert823", "/uploads/v1/user/13466806.b5833334.161x161o.3e35f6772f49.jpeg"},
			{"McGoohan", "/uploads/v1/user/8924596.2ec65f06.161x161o.f02ae8db3e6f.jpeg"},
			{"JohnHS", "/uploads/v1/user/26388524.7082f084.161x161o.8f66fa428182.png"},
			{"SuperSam1", "/uploads/v1/user/30832834.bce39087.161x161o.b436260ada55.jpeg"},
			{"landontu", "/uploads/v1/user/23695394.8fea66df.161x161o.c4526e8c2c47.jpeg"},
			{"JamesAgadir", "/uploads/v1/user/17462676.280270de.161x161o.17fa29abe01b.jpeg"},
			{"PhillipCarlyle", "/uploads/v1/user/53157930.08894836.161x161o.a60d59b7eb9c.png"},
			{"Benjibass", "/uploads/v1/user/42136568.e5f8c040.161x161o.0342a4638dfa.png"},
			{"clbamonica", "/uploads/v1/user/32570210.a91d162e.161x161o.2d33f6d637f5.jpeg"},
			{"GustavKlimtPaints", "/uploads/v1/user/28956220.4d71d14f.161x161o.c3f46739b3b6.jpeg"},
			{"aa-ron1235", "/uploads/v1/user/30133316.481b027b.161x161o.7db0b6285736.jpeg"},
			{"HorribleTomato", "/uploads/v1/user/35609232.70de4195.161x161o.b4bfe0cca386.png"},
			{"hitthepin", "/uploads/v1/user/39917346.da18e49c.161x161o.5c850ce491ed.png"},
			{"Critica7", "/uploads/v1/user/37890288.d3d8464f.161x161o.4d1f3c4c4856.jpeg"},
			{"Waredude", "/uploads/v1/user/38010254.6ad47363.161x161o.4831b528e164.png"},
			{"aidanB1", "/uploads/v1/user/27103064.7135d0d1.161x161o.d95d43eba350.jpeg"},
			{"ScootaChess", "/uploads/v1/user/41081120.26c8b389.161x161o.c2aa3ff5c8ae.jpeg"},
			{"Fly-Eagles-Fly", "/uploads/v1/user/43864698.096016fc.161x161o.51fb3cd77178.jpeg"},
			{"togepi2468", "/uploads/v1/user/34775460.455ae7f4.161x161o.4e352f77c13c.png"},
			{"MasterMaceWindu", "/uploads/v1/user/29506860.3959f8c2.161x161o.28e47046e00d.jpeg"},
			{"silvertruck", "/uploads/v1/user/37841308.33be861b.161x161o.102f3188b344.png"},
			{"Balashukla0761", "/uploads/v1/user/19727420.9669966b.161x161o.d8f12ad4b239.png"},
			{"josephyossi", "/uploads/v1/user/28566960.2c98a004.161x161o.b935d56c631c.jpeg"},
			{"unc_the_awesome", "/uploads/v1/user/37503058.153a76c7.161x161o.092b47fcf756.jpeg"},
			{"vickalan", "/uploads/v1/user/31356306.7c83cbc9.161x161o.e667b4765b72.jpeg"},
			{"Junebug444", "/uploads/v1/user/43533448.cb6d3379.161x161o.aae4a278dd45.png"},
			{"foofooes", "/uploads/v1/user/28906064.de88577d.161x161o.cb6a01b9134c.jpeg"}
		};
	};
}