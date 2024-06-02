#ifndef ZKP_VOTING_HPP
#define ZKP_VOTING_HPP

#include <string>
#include <libsnark/common/default_types/r1cs_ppzksnark_pp.hpp>
#include <libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp>

class ZKPVoting {
public:
    ZKPVoting();
    ~ZKPVoting();

    void generate_and_OutputProof(const std::string &nickname, const std::string &candidate); // Generate and outpu the proof of voting
    bool verify_Proof(const std::string &nickname, const std::string &candidate); // Verifying the vote
    void updateResults(const std::string &candidate); // Updating rhe voting records

    int get_VotesAlice() const; // Get the votes of Alice
    int get_VotesBob() const; // Get the votes of Bob

private:
    // Typedefing for proving and verification keys
    typedef libsnark::r1cs_ppzksnark_proving_key<libsnark::default_r1cs_ppzksnark_pp> proving_key_t;
    typedef libsnark::r1cs_ppzksnark_verification_key<libsnark::default_r1cs_ppzksnark_pp> verification_key_t;

    proving_key_t proving_key;
    verification_key_t verification_key;
    libsnark::r1cs_constraint_system<libff::Fr<libsnark::default_r1cs_ppzksnark_pp>> cs;

    int votesAlice;
    int votesBob;

    void generateKeys(); // Generating keys
    libsnark::r1cs_ppzksnark_keypair<libsnark::default_r1cs_ppzksnark_pp> keypair;


};
#endif
