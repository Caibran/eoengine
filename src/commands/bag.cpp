/* $Id$
 * EOSERV is released under the zlib license.
 * See LICENSE.txt for more info.
 */

#include "commands.hpp"

#include "../character.hpp"
#include "../config.hpp"
#include "../eoplus.hpp"
#include "../i18n.hpp"
#include "../packet.hpp"
#include "../quest.hpp"
#include "../world.hpp"

#include "../util.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace Commands
{

void Bag(const std::vector<std::string>& arguments, Character* from)
{
	// Open the bag interface - reuse the locker packet format
	// Use special coordinates (255, 255) to indicate this is a bag operation
	PacketBuilder reply(PACKET_LOCKER, PACKET_OPEN, 2 + from->bag.size() * 5);
	reply.AddChar(255); // Special x coordinate to indicate bag
	reply.AddChar(255); // Special y coordinate to indicate bag
	UTIL_FOREACH(from->bag, item)
	{
		reply.AddShort(item.id);
		reply.AddThree(item.amount);
	}
	from->Send(reply);
}

COMMAND_HANDLER_REGISTER(bag)
	RegisterCharacter({"bag", {}, {}, 3}, Bag);
COMMAND_HANDLER_REGISTER_END(bag)

}