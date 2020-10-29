enum rarity
{
	common,
	uncommon,
	rare,
	mythic
};

typedef struct
{
	unsigned int id;
	char* name;
	char* cost;
	unsigned int converted_cost;
	char* type;
	char* text;
	char* stats;
	enum rarity rarity;
}Card;

typedef struct
{
	u_int32_t nameLength;
	char* name;
	long offset;
}Offset;