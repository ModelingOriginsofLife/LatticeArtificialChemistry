class Reaction
{
public:
    char a_type,b_type,c_type;
    int a_state,b_state,c_state;
    int future_a_state,future_b_state,future_c_state;
    bool current_ab_bond,current_bc_bond,current_ac_bond;
    bool future_ab_bond,future_bc_bond,future_ac_bond;

    int n_inputs;

    // for 2-atom reactions:
    Reaction(char a_type,int a_state,bool current_ab_bond,char b_type,int b_state,
            int future_a_state,bool future_ab_bond,int future_b_state)
    {
        this->a_type=a_type;
        this->a_state=a_state;
        this->current_ab_bond=current_ab_bond;
        this->b_type=b_type;
        this->b_state=b_state;
        this->future_a_state=future_a_state;
        this->future_ab_bond=future_ab_bond;
        this->future_b_state=future_b_state;
        this->n_inputs=2;
    }
    
    // for 3-atom reactions:
    Reaction(char a_type,int a_state,bool current_ab_bond,
            char b_type,int b_state,bool current_bc_bond,
            char c_type,int c_state,bool current_ac_bond,
            int future_a_state,bool future_ab_bond,
            int future_b_state,bool future_bc_bond,
            int future_c_state,bool future_ac_bond)
    {
        this->a_type=a_type;
        this->a_state=a_state;
        this->current_ab_bond=current_ab_bond;
        this->b_type=b_type;
        this->b_state=b_state;
        this->current_bc_bond=current_bc_bond;
        this->c_type=c_type;
        this->c_state=c_state;
        this->current_ac_bond=current_ac_bond;
        this->future_a_state=future_a_state;
        this->future_ab_bond=future_ab_bond;
        this->future_b_state=future_b_state;
        this->future_bc_bond=future_bc_bond;
        this->future_c_state=future_c_state;
        this->future_ac_bond=future_ac_bond;
        this->n_inputs=3;
    }
    
    static bool isVariable(char c) { return c=='x' || c=='y' || c=='z'; }
};

