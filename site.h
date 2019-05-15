#ifndef _SITE_H_
#define _SITE_H_

class Site {
private:
    std::vector<std::vector<Fish*> > fish;

public:
    Site();
    ~Site();

    Fish *get_fish(const int fish_id, const int index) const;
    void add_fish(Fish *fish);
    void del_fish(const int fish_id, const int index, const bool kill);
    int get_count(const int fish_id) const; 
};

#endif