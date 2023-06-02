#define map "Red1", "Red2", "\n", "Blue1", "Blue2"

typedef enum autonSelect {
	RED1 = 1,
    RED2,
    BLUE1,
    BLUE2
} autonSelectT;

autonSelectT getAutonChoice();

void btnmatrix();