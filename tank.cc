#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>
#include "fish.h"
#include "site.h"
#include "tank.h"

Tank::Tank(){
    Fish *tmp;
    int m=150, t=10, s=5;
    tot_count[0]=m; tot_count[1]= t; tot_count[2]=s;
    
    // allocating and randomly adding Minnows, Tuna & Sharks //
    // also testing for allocation errors //
    try {
        for(int i=0;i<m;i++){
            tmp = new Minnow();         // tests here
            tank[rand()%5][rand()%5][rand()%5].add_fish(tmp);
        }
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Minnows" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    
    try {
        for(int i=0;i<t;i++){
            tmp = new Tuna();         // tests here
            tank[rand()%5][rand()%5][rand()%5].add_fish(tmp);
        }
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Tuna" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }

    try {
        for(int i=0;i<s;i++){
            tmp = new Shark();         // tests here
            tank[rand()%5][rand()%5][rand()%5].add_fish(tmp);
        }
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Sharks" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
}

// non-deafault contructor // 
Tank::Tank(const int minnows, const int tuna, const int sharks){
    assert(minnows > 0 && tuna > 0 && sharks > 0);

    Fish *tmp;
    int m=minnows, t=tuna, s=sharks;
    
    tot_count[0]=m; tot_count[1]= t; tot_count[2]=s;
    
    try {
        tot_count[0]=m; tot_count[1]= t; tot_count[2]=s;
        for(int i=0;i<m;i++){
            tmp = new Minnow();         // tests here
            tank[rand()%5][rand()%5][rand()%5].add_fish(tmp);
        }
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Minnows" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    
    try {
        for(int i=0;i<t;i++){
            tmp = new Tuna();         // tests here
            tank[rand()%5][rand()%5][rand()%5].add_fish(tmp);
        }
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Tuna" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }

    try {
        for(int i=0;i<s;i++){
            tmp = new Shark();         // tests here
            tank[rand()%5][rand()%5][rand()%5].add_fish(tmp);
        }
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Tuna" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
}

// returns ptr to Site ~ location site //
const Site* Tank::get_site(const int *site) const {
    const Site* tmp = &tank[site[0]][site[1]][site[2]];

    return tmp;
}

// returns total number of fish of type fish_id in existence //
int Tank::get_count(const int fish_id) const {
    assert(fish_id >=0 && fish_id < 3);

    int count = tot_count[fish_id];

    return count;
}

// checks if a certain type of fish exists at certain site //
bool Tank::fish_present(const int *site, const int fish_id) const {
    assert(fish_id >=0 && fish_id < 3);
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    const Site *src;
    
    src = &tank[site[0]][site[1]][site[2]];
     
    if(src->get_count(fish_id) > 0)
        return 1;
    else
        return 0;
}

// prints grid //
void Tank::print() const {
    const Site *tmp;

    for(int k=0;k<5;k++){
        for(int i=0;i<5;i++){
            for(int j=0;j<5;j++){
                tmp = &tank[i][j][k];
                std::cout << "(" << tmp->get_count(0) << "," << tmp->get_count(1) << "," << tmp->get_count(2) << ")";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
}

// performs a single iteration of a sweep //
void Tank::sweep(){
    int fish_id, p_move, num_out;;
    int src[3], dest[3];
    void (Tank::*outcome_fns[6])(const int *);  // will store at most 6 fns. stores plausible //
                                                // outcomes after move //
    
    // if one fish gone extinct, exit //
    if(tot_count[0]==0 || tot_count[1]==0 || tot_count[2]==0){
        std::cout << std::endl << "FISH DIED OUT." << std::endl << std::endl;
        exit(0);
    } 
    
    // pick type of fish at random //
    fish_id = rand()%3;
    // randomly choose Sites until one with that fish type is found //
    while(true){
        src[0] = rand()%5; src[1] = rand()%5; src[2] = rand()%5;
        if(fish_present(src, fish_id))
                break;
    }

    // probability of moving //
    p_move = rand()%10;
     
    // 9/10 chance p_move != 0 //
    if(p_move!=0)
        move(src, dest, fish_id);
    else    // otherwise dest = src //
        dest[0] = src[0], dest[1] = src[1], dest[2] = src[2];
    
    // finds number of possible outcomes at dest and adds //
    // the plausible choices to arr of fn ptrs //
    num_out = check_outcomes(dest, outcome_fns);
    // if num_out != 0, apply one of possible outcomes at random with equal odds //
    if(num_out)
        (this->*outcome_fns[rand()%num_out])(dest);
}

// performs move of fish, from src to dest //
void Tank::move(const int *site, int *dest_coords, const int fish_id){
    assert(fish_id >=0 && fish_id < 3);
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Site *src, *dest;
    Fish *fish_tmp;
    int count, rand_fish;
    
    src = &tank[site[0]][site[1]][site[2]];
    
    // counts amount of fish of type fish_id at Site src //
    count = src->get_count(fish_id);

    // picks fish at random //
    rand_fish = rand()%count;
    try {
        fish_tmp = src->get_fish(fish_id, rand_fish);
    } catch (std::range_error const &err) {
        std::cerr << "Index of Fish on site out of range" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
   
    // applies fish's move function //
    fish_tmp->move(site, dest_coords);
    
    // if tuna/sharks moved more than 4 times without eating //
    if(fish_tmp->get_moves()>4 && (fish_id==1 || fish_id==2)){
        src->del_fish(fish_id, rand_fish, 1);
        tot_count[fish_id]--;
        return;
    }

    // pointing to dest Site //
    dest = &tank[dest_coords[0]][dest_coords[1]][dest_coords[2]];
    
    // adding fish to dest vector //
    dest->add_fish(fish_tmp);
    // removing fish from old vector //
    try {
        src->del_fish(fish_id, rand_fish, 0);
    } catch (std::range_error const &err) {
        std::cerr << "Index of Fish on site out of range." << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
}

// checks what outcomes possible at site and adds their fn to fn_ptr array //
// returns total number of possible outcomes (out of 6) //
int Tank::check_outcomes(const int *site, void (Tank::**outcome_fns)(const int *)){
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Site *src;
    int index;

    src = &tank[site[0]][site[1]][site[2]];
    index = 0;
    
    // 2 minnows present //    
    if(src->get_count(0) > 1){
        outcome_fns[index] = &Tank::minnow_breed;
        index++;
    }
    // 2 tuna present //
    if(src->count_fed(1) > 1){
        outcome_fns[index] = &Tank::tuna_breed;
        index++;
    }
    // 2 sharks present //
    if(src->count_fed(2) > 1){
        std::cout << "fed count = " << src->count_fed(2) << std::endl;
        outcome_fns[index] = &Tank::shark_breed;
        index++;
    }
    // minnow & tuna //
    if(fish_present(site, 0) && fish_present(site, 1)){
        outcome_fns[index] = &Tank::tuna_feed;
        index++;
    }
    // shark & tuna //
    if(fish_present(site, 1) && fish_present(site, 2)){
        outcome_fns[index] = &Tank::shark_feed;
        index++;
    }
    // feeding frenzy //
    if(fish_present(site,0) && fish_present(site,2)){
        outcome_fns[index] = &Tank::feeding_frenzy;
        index++;
    }

    return index;
}

///////////// OUTCOME FUNCTIONS ///////////////////

void Tank::minnow_breed(const int *site){
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Fish *tmp;
    
    std::cout << "minnow breed" << std::endl;
    
    try {
        for(int i=0;i<3;i++){
            tmp = new Minnow();
            tank[site[0]][site[1]][site[2]].add_fish(tmp);
        }
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Minnows" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    
    tot_count[0] += 3;
}

void Tank::tuna_breed(const int *site){
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Fish *tmp;
    
    std::cout << "tuna breed" << std::endl;

    try {
        tmp = new Tuna();
        tank[site[0]][site[1]][site[2]].add_fish(tmp);
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Tuna" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    

    tot_count[1]++;
}

void Tank::shark_breed(const int *site){
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Fish *tmp;
    
    std::cout << "Shark breed" << std::endl;
    
    try {
        tmp = new Shark();
        tank[site[0]][site[1]][site[2]].add_fish(tmp);
    } catch (std::bad_alloc const &err) {
        std::cerr << "Error in allocating Shark" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    
    tot_count[2]++;
}

void Tank::tuna_feed(const int *site){
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Site *src;

    std::cout << "Tuna feed" << std::endl;
    
    src = &tank[site[0]][site[1]][site[2]];

    try {
        tot_count[0] -= src->kill_fish(0);
    } catch(std::exception const &err){
        std::cerr << "Error deleting Minnows" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    
    src->feed_fish(1);
}

void Tank::shark_feed(const int *site){
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Site *src;
    int count;

    std::cout << "Shark feed" << std::endl;
    
    src = &tank[site[0]][site[1]][site[2]];
    count = src->get_count(1);

    try {
        src->del_fish(1, rand()%count, 1);
    } catch(std::exception const &err){
        std::cerr << "Error deleting Tuna" << std::endl;
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    
    tot_count[1] -= 1;

    src->feed_fish(2);
}

void Tank::feeding_frenzy(const int *site){
    assert(site[0] < 5 && site[0] >=0 && site[1] < 5 && 
                site[1] >= 0 && site[2] < 5 && site[2] >=0);
    
    Site *src, *nbr_site;
    int nbr[3];
    
    std::cout << "Feeding frenzy" << std::endl;

    src = &tank[site[0]][site[1]][site[2]];
    
    for(int i=-1;i<2;i++){
        nbr[0] = ((( site[0]+i )%5) + 5 )%5;
        for(int j=-1;j<2;j++){
            nbr[1] = ((( site[1]+j )%5) + 5 )%5;
            for(int k=-1;k<2;k++){            
                nbr[2] = ((( site[2]+k )%5) + 5 )%5;
                nbr_site = &tank[nbr[0]][nbr[1]][nbr[2]];
                if(fish_present(nbr, 0)){
                    try {
                        tot_count[0] -= nbr_site->kill_fish(0);
                    } catch(std::exception const &err){
                        std::cerr << "Error deleting Minnows during frenzy" << std::endl;
                        std::cerr << err.what() << std::endl;
                        exit(1);
                    }
                }
            }
        }
    }
    
    src->feed_fish(2);
}


// overloading << operator to replace print //
std::ostream &operator<<(std::ostream &os, const Tank &tank){
    const Site *tmp;
    int src[3];

    for(int k=0;k<5;k++){
        src[2] = k;
        for(int i=0;i<5;i++){
            src[0] = i;
            for(int j=0;j<5;j++){
                src[1] = j;
                tmp = tank.get_site(src);
                os << "(" << tmp->get_count(0) << "," << tmp->get_count(1) << "," << tmp->get_count(2) << ")";
            }
            os << std::endl;
        }
        os << std::endl << std::endl;
    }
    
    return os;
}
