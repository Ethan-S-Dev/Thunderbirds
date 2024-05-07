enum class MoveFailReason : int {
	HitWall,
	HitShip
};

struct MoveResult {
	bool CanBeMoved;
	union {
		MoveFailReason FailReason;
		int MoveCapacityCost;
	};
};