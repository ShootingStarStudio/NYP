#include "Inventory.h"

Inventory::Inventory()
{

}

Inventory::~Inventory()
{

}

std::vector<std::string> Inventory::GetInGameItems()
{
	return items;
}

void Inventory::AddInGameItem(std::string itemName)
{
	if (items.size() < 10) { items.push_back(itemName); }
}

void Inventory::RemoveItem(std::string str)
{
	for (unsigned i = 0; i < items.size(); i++)
	{
		if (items[i] == str)
		{
			items.erase(items.begin() + i);
			break;
		}
	}
}