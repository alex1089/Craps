// Aleksey Leshchuk
// Cisp440
// Craps Simulator, runs 10000 games and displays stats

#include <iostream>
#include <random> // random number generator
#include <iomanip>
using namespace std;

// enum for win on first roll at XX
enum FirstRollWin { FWSEVEN, FWELEVEN };
enum FirstRollLoss { FLTWO, FLTHREE, FLTWELVE };
// enum for result of first roll
enum FirstRollResult { WON, LOST, POINT };
// enumeration for point scored for readability
enum Points { FOUR, FIVE, SIX, EIGHT, NINE, TEN};

class Craps {
    public:
	Craps():
	    total_wins(0),
	    total_losses(0),
	    longest_run(0)
	{
	    init();
	    for (int i=0; i<10000;i++)
		firstRoll();
	    printStats();
	}
	// simulate dice throw
	int rollDice() {
	    int roll;
    	    static default_random_engine gen;		    // random number generator
	    static uniform_int_distribution<int> dist(1,6);	    // range from [1,6]
	    roll = dist(gen)+dist(gen);
	    dice_rolls[roll-2]++;		    // increment the roll counter for X outcome
	    return roll;
	}
	void firstRoll() {
	    int roll=rollDice();	// roll the dice
	    switch (roll){
		case 7:
		case 11:
		    first_roll_result[WON]++;
		    win(roll,true);		    // win on 'roll', first roll? true
		    break;
		case 2:
		case 3:
		case 12:
		    first_roll_result[LOST]++;
		    loss(roll, true);		    // lost on 'roll' , first roll? true
		    break;
		default:
		    first_roll_result[POINT]++;
		    pointRoll(roll);		    // continue game with point
		    break;
	    }
	}
	// called on scoring of point
	void pointRoll(int point) {
	    // called when point scored
	    bool valid_roll=false;
	    int roll;
	    do {	// roll until the point is rolled, or until 7(lost)
		roll=rollDice(); 
		if (roll == 7){
		    loss(point,false);	    // lost with 'point', not first roll
		    valid_roll=true;
		} else if (roll == point) {	
		    win(point, false);	    // won with 'point', not first roll
		    valid_roll=true;
		}
	    } while (!valid_roll);
		    
	}
	// call on win
	void win(int dice_roll, bool first_roll) {
	    total_wins++;	// increment wins
	    if (first_roll) {	// first roll case
		   if (dice_roll == 7)
		       first_throw_wins[FWSEVEN]++;	// first throw win++ with FirstWinSEVEN/ELEVEN
		   else 
		       first_throw_wins[FWELEVEN]++; 
	    } else {	// if not the first roll	
		switch (dice_roll){
		    case 4:
			win_with_point[FOUR]++;	// increment win with point XXXX
			break;
		    case 5:
			win_with_point[FIVE]++;
			break;
		    case 6:
			win_with_point[SIX]++;
			break;
		    case 8:
			win_with_point[EIGHT]++;
			break;
		    case 9:
			win_with_point[NINE]++;
			break;
		    case 10:
			win_with_point[TEN]++;
			break;
		}
	    }
	}
	// called on loss
	void loss(int dice_roll, bool first_roll) {
	    total_losses++;	    // increment total losses
	    if (first_roll) {	    // on first roll 
		switch (dice_roll) {	// increment loss with point X
		    case 2:
			first_throw_losses[FLTWO]++;
			break;
		    case 3:
			first_throw_losses[FLTHREE]++;
			break;
		    case 4:
			first_throw_losses[FLTWELVE]++;
			break;
		}
	    } else {		    // not first roll
		switch(dice_roll) {	// increment loss with point X 
		    case 4:
			loss_with_point[FOUR]++;	// increment win with point XXXX
			break;
		    case 5:
			loss_with_point[FIVE]++;
			break;
		    case 6:
			loss_with_point[SIX]++;
			break;
		    case 8:
			loss_with_point[EIGHT]++;
			break;
		    case 9:
			loss_with_point[NINE]++;
			break;
		    case 10:
			loss_with_point[TEN]++;
			break;
		}
	    }
	}
	// print stats of all games
	void printStats(){
	    int total_dice_rolls=0;
	    int total_rolls=total_wins+total_losses;

	    cout<<"Dice Roll Distribution: \n";
	    for (int i: dice_rolls)
		total_dice_rolls+=i;
	    for (int i=0; i<11;i++)	    // display stat for dice roll distribution
		cout<<fixed<<setprecision(0)<<"Roll: "<<i+2<<" = "<<static_cast<double>(dice_rolls[i])/total_dice_rolls*100<<"%"<<endl;
	    
	    // total wins/loses
	    cout<<"Total Wins:\n"<<setw(25)<<static_cast<double>(total_wins)/total_rolls*100<<"%\n";
	    cout<<"Total Loses: \n"<<setw(25)<<static_cast<double>(total_losses)/total_rolls*100<<"%\n";

	    
	    // distribution of first roll results
	    cout<<"Results of the first roll: \n";
	    cout<<"Won: "<<static_cast<double>(first_roll_result[WON])/total_rolls*100<<"%"<<endl;
	    cout<<"Lost: "<<static_cast<double>(first_roll_result[LOST])/total_rolls*100<<"%"<<endl;
	    cout<<"Point: "<<static_cast<double>(first_roll_result[POINT])/total_rolls*100<<"%"<<endl;

	    // distribution of points received
	    cout<<"\nDistribution of points:\n";
	    for (int i=0; i<6;i++){
		cout<<"Point: ";
		switch (i) {
		    case 0:
			cout<<"FOUR ";
			break;
		    case 1:
			cout<<"FIVE ";
			break;
		    case 2:
			cout<<"SIX ";
			break;
		    case 3:
			cout<<"EIGHT ";
			break;
		    case 4:
			cout<<"NINE ";
			break;
		    case 5:
			cout<<"TEN ";
			break;
		}
		cout<<": "<<static_cast<double>(win_with_point[i]+loss_with_point[i])/first_roll_result[POINT]*100<<"%"<<endl;
		cout<<setw(14)<<"Won: "<<static_cast<double>(win_with_point[i])/(win_with_point[i]+loss_with_point[i])*100<<"%"<<endl;
		cout<<setw(14)<<"Lost: "<<static_cast<double>(loss_with_point[i])/(win_with_point[i]+loss_with_point[i])*100<<"%"<<endl;
	    }



	}
    private:
	// initializes all array variables
	void init(){
	    for (int& i:dice_rolls)
		i=0;
	    for (int& i: first_roll_result)
		i=0;
	    for (int& i:first_throw_wins)
		i=0;
	    for (int& i:first_throw_losses)
		i=0;
	    for (int& i:win_with_point)
		i=0;
	    for (int& i:loss_with_point)
		i=0;
	}
	int dice_rolls[11];	    // stats for dice rolls
	int first_roll_result[3];
	int total_wins;		    // total wins
	int total_losses;	    // total losses
	int first_throw_wins[2];	    // wins on first throw at point X
	int first_throw_losses[3];	    // losses on first throw at X 
	int win_with_point[6];	    // win with point x
	int loss_with_point[6];	    // los with point x
	int longest_run;	    // keep track of longest run
	
};
	
	
