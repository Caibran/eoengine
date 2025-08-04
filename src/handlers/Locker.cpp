
/* $Id$
 * EOSERV is released under the zlib license.
 * See LICENSE.txt for more info.
 */

#include "handlers.hpp"

#include "../character.hpp"
#include "../config.hpp"
#include "../eodata.hpp"
#include "../eoserver.hpp"
#include "../map.hpp"
#include "../packet.hpp"
#include "../world.hpp"

#include "../util.hpp"

#include <algorithm>
#include <cstddef>

namespace Handlers
{

static PacketBuilder add_common(Character *character, short item, int amount)
{
	character->DelItem(item, amount);

	character->CalculateStats();

	PacketBuilder reply(PACKET_LOCKER, PACKET_REPLY, 8 + character->bank.size() * 5);
	reply.AddShort(item);
	reply.AddInt(character->HasItem(item));
	reply.AddChar(static_cast<unsigned char>(character->weight));
	reply.AddChar(static_cast<unsigned char>(character->maxweight));

	UTIL_FOREACH(character->bank, item)
	{
		reply.AddShort(item.id);
		reply.AddThree(item.amount);
	}

	return reply;
}

// Placing an item in a bank locker or bag  
void Locker_Add(Character *character, PacketReader &reader)
{
	if (character->trading) return;

	unsigned char x = reader.GetChar();
	unsigned char y = reader.GetChar();
	short item = reader.GetShort();
	int amount = reader.GetThree();

	if (item == 1) return;
	if (amount <= 0) return;
	if (character->HasItem(item) < amount) return;

	// Check if this is a bag operation (special coordinates 255, 255)
	if (x == 255 && y == 255)
	{
		// Bag operation
		std::size_t bagmax = 3 + (character->baglevel - 1);

		UTIL_IFOREACH(character->bag, it)
		{
			if (it->id == item)
			{
				if (it->amount + amount < 0)
				{
					return;
				}

				amount = std::min<int>(amount, static_cast<int>(character->world->config["MaxBank"]) - it->amount);

				it->amount += amount;

				character->DelItem(item, amount);
				character->CalculateStats();

				PacketBuilder reply(PACKET_LOCKER, PACKET_REPLY, 8 + character->bag.size() * 5);
				reply.AddShort(item);
				reply.AddInt(character->HasItem(item));
				reply.AddChar(static_cast<unsigned char>(character->weight));
				reply.AddChar(static_cast<unsigned char>(character->maxweight));

				UTIL_FOREACH(character->bag, bagitem)
				{
					reply.AddShort(bagitem.id);
					reply.AddThree(bagitem.amount);
				}

				character->Send(reply);
				return;
			}
		}

		if (character->bag.size() >= bagmax)
		{
			PacketBuilder reply(PACKET_LOCKER, PACKET_SPEC);
			reply.AddChar(static_cast<unsigned char>(bagmax));
			character->Send(reply);
			return;
		}

		amount = std::min<int>(amount, static_cast<int>(character->world->config["MaxBank"]));

		Character_Item newitem;
		newitem.id = item;
		newitem.amount = amount;

		character->bag.push_back(newitem);

		character->DelItem(item, amount);
		character->CalculateStats();

		PacketBuilder reply(PACKET_LOCKER, PACKET_REPLY, 8 + character->bag.size() * 5);
		reply.AddShort(item);
		reply.AddInt(character->HasItem(item));
		reply.AddChar(static_cast<unsigned char>(character->weight));
		reply.AddChar(static_cast<unsigned char>(character->maxweight));

		UTIL_FOREACH(character->bag, bagitem)
		{
			reply.AddShort(bagitem.id);
			reply.AddThree(bagitem.amount);
		}

		character->Send(reply);
		return;
	}

	// Normal locker operation
	std::size_t lockermax = static_cast<int>(character->world->config["BaseBankSize"]) + character->bankmax * static_cast<int>(character->world->config["BankSizeStep"]);

	if (util::path_length(character->x, character->y, x, y) <= 1)
	{
		if (character->map->GetSpec(x, y) == Map_Tile::BankVault)
		{
			UTIL_IFOREACH(character->bank, it)
			{
				if (it->id == item)
				{
					if (it->amount + amount < 0)
					{
						return;
					}

					amount = std::min<int>(amount, static_cast<int>(character->world->config["MaxBank"]) - it->amount);

					it->amount += amount;

					PacketBuilder reply = add_common(character, item, amount);
					character->Send(reply);
					return;
				}
			}

			if (character->bank.size() >= lockermax)
			{
				PacketBuilder reply(PACKET_LOCKER, PACKET_SPEC);
				reply.AddChar(static_cast<unsigned char>(lockermax));
				character->Send(reply);
				return;
			}

			amount = std::min<int>(amount, static_cast<int>(character->world->config["MaxBank"]));

			Character_Item newitem;
			newitem.id = item;
			newitem.amount = amount;

			character->bank.push_back(newitem);

			PacketBuilder reply = add_common(character, item, amount);
			character->Send(reply);
		}
	}
}

// Taking an item from a bank locker or bag
void Locker_Take(Character *character, PacketReader &reader)
{
	if (character->trading) return;

	unsigned char x = reader.GetChar();
	unsigned char y = reader.GetChar();
	short item = reader.GetShort();

	// TODO: Limit number of items withdrawn to under weight

	// Check if this is a bag operation (special coordinates 255, 255)
	if (x == 255 && y == 255)
	{
		// Bag operation
		UTIL_IFOREACH(character->bag, it)
		{
			if (it->id == item)
			{
				int amount = it->amount;
				int taken = character->CanHoldItem(it->id, amount);

				character->AddItem(item, taken);
				character->CalculateStats();

				PacketBuilder reply(PACKET_LOCKER, PACKET_GET, 7 + character->bag.size() * 5);
				reply.AddShort(item);
				reply.AddThree(taken);
				reply.AddChar(static_cast<unsigned char>(character->weight));
				reply.AddChar(static_cast<unsigned char>(character->maxweight));

				it->amount -= taken;

				if (it->amount <= 0)
					character->bag.erase(it);

				UTIL_FOREACH(character->bag, bagitem)
				{
					reply.AddShort(bagitem.id);
					reply.AddThree(bagitem.amount);
				}
				character->Send(reply);

				break;
			}
		}
		return;
	}

	// Normal locker operation
	if (util::path_length(character->x, character->y, x, y) <= 1)
	{
		if (character->map->GetSpec(x, y) == Map_Tile::BankVault)
		{
			UTIL_IFOREACH(character->bank, it)
			{
				if (it->id == item)
				{
					int amount = it->amount;
					int taken = character->CanHoldItem(it->id, amount);

					character->AddItem(item, taken);

					character->CalculateStats();

					PacketBuilder reply(PACKET_LOCKER, PACKET_GET, 7 + character->bank.size() * 5);
					reply.AddShort(item);
					reply.AddThree(taken);
					reply.AddChar(static_cast<unsigned char>(character->weight));
					reply.AddChar(static_cast<unsigned char>(character->maxweight));

					it->amount -= taken;

					if (it->amount <= 0)
						character->bank.erase(it);

					UTIL_FOREACH(character->bank, item)
					{
						reply.AddShort(item.id);
						reply.AddThree(item.amount);
					}
					character->Send(reply);

					break;
				}
			}
		}
	}
}

// Opening a bank locker
void Locker_Open(Character *character, PacketReader &reader)
{
	unsigned char x = reader.GetChar();
	unsigned char y = reader.GetChar();

	if (util::path_length(character->x, character->y, x, y) <= 1)
	{
		if (character->map->GetSpec(x, y) == Map_Tile::BankVault)
		{
			PacketBuilder reply(PACKET_LOCKER, PACKET_OPEN, 2 + character->bank.size() * 5);
			reply.AddChar(x);
			reply.AddChar(y);
			UTIL_FOREACH(character->bank, item)
			{
				reply.AddShort(item.id);
				reply.AddThree(item.amount);
			}
			character->Send(reply);
		}
	}
}

// Purchasing a locker space upgrade
void Locker_Buy(Character *character, PacketReader &reader)
{
	if (character->trading) return;

	(void)reader;

	if (character->npc_type == ENF::Bank)
	{
		int cost = static_cast<int>(character->world->config["BankUpgradeBase"]) + character->bankmax * static_cast<int>(character->world->config["BankUpgradeStep"]);

		if (character->bankmax >= static_cast<int>(character->world->config["MaxBankUpgrades"]))
		{
			return;
		}

		if (character->HasItem(1) < cost)
		{
			return;
		}

		++character->bankmax;
		character->DelItem(1, cost);

		PacketBuilder reply(PACKET_LOCKER, PACKET_BUY, 5);
		reply.AddInt(character->HasItem(1));
		reply.AddChar(character->bankmax);
		character->Send(reply);
	}
}

PACKET_HANDLER_REGISTER(PACKET_LOCKER)
	Register(PACKET_ADD, Locker_Add, Playing);
	Register(PACKET_TAKE, Locker_Take, Playing);
	Register(PACKET_OPEN, Locker_Open, Playing);
	Register(PACKET_BUY, Locker_Buy, Playing);
PACKET_HANDLER_REGISTER_END(PACKET_LOCKER)

}
