local _M = {}

local function newNode(x, y)
	return {
		x = x,
		y = y,
		g = 0,
		h = 0,
		f = 0,
		parent = nil,
	}
end

local function hash(x, y)
	return y * 1000 + x
end

local function manhattanDis(a, b)
	return math.abs(a.x - b.x) + math.abs(a.y - b.y)
end

--左右上下，四个方向
local EMoveDir = {
	{-1, 0}, {1, 0}, {0, -1}, {0, 1}
}

local function aStar(map, startPos, endPos)
	local openSet = { }
	local closedSet = { }
	local startNode = newNode(startPos[1], startPos[2])
	local endNode = newNode(endPos[1], endPos[2])

	startNode.h = manhattanDis(startNode, endNode)
	startNode.f = startNode.h

	openSet[hash(startNode.x, startNode.y)] = startNode

	while next(openSet) do
		local current
		local lowsetF = math.huge
		for index, node in pairs(openSet) do
			if node.f < lowsetF then
				current = node
				lowsetF = node.f
			end
		end

		if current == nil then
			return
		end

		if current.x == endNode.x and current.y == endNode.y then
			return current
		end

		local h = hash(current.x, current.y)
		closedSet[h] = current
		openSet[h] = nil

		for _, dir in pairs(EMoveDir) do
			local x, y = dir[1], dir[2]
			local neighbor = newNode(current.x + x, current.y + y)

			if neighbor.x == endNode.x and neighbor.y == endNode.y then
				neighbor.parent = current
				return neighbor
			end

			local walkable = (map[neighbor.y] and map[neighbor.y][neighbor.x] == 0) and closedSet[hash(neighbor.x, neighbor.y)] == nil
			if walkable then
				neighbor.g = current.g + 1
				neighbor.h = manhattanDis(neighbor, endNode)
				neighbor.f = neighbor.g + neighbor.h

				local inOpenNode = openSet[hash(neighbor.x, neighbor.y)]
				if inOpenNode then
					if (neighbor.f < inOpenNode.f) then
						inOpenNode.parent = current
						inOpenNode.f = neighbor.f
					end
				else
					neighbor.parent = current
					openSet[hash(neighbor.x, neighbor.y)] = neighbor
				end
			end
		end
	end
end

function _M.FindPath(map, startPos, endPos)
	local path = aStar(map, startPos, endPos)
	if not path then
		return
	end

	local temp = {}
	while path.parent do
		table.insert(temp, path)
		path = path.parent
	end
	
	if #temp == 0 then
		return
	end

	--翻转路径节点
	local result = {}
	for i = #temp, 1, -1 do
		temp[i].parent = nil
		table.insert(result, temp[i])
	end

	return result
end

return _M