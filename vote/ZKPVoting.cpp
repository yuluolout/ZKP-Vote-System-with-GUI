#include "ZKPVoting.hpp"
#include <iostream>
#include <fstream>
#include <libsnark/gadgetlib1/protoboard.hpp>
#include <libsnark/relations/constraint_satisfaction_problems/r1cs/r1cs.hpp>
#include <libff/common/utils.hpp>
#include <libff/common/profiling.hpp>

using namespace libsnark;
using namespace libff;
using namespace std;

ZKPVoting::ZKPVoting() : votesAlice(0), votesBob(0) {
    default_r1cs_ppzksnark_pp::init_public_params();
    generateKeys();
}

ZKPVoting::~ZKPVoting() {}

void ZKPVoting::generateKeys() {
    typedef Fr<default_r1cs_ppzksnark_pp> FieldT;

    // Create a protoboard
    protoboard<FieldT> pb;

    // Declare variables
    pb_variable<FieldT> voter_id;
    pb_variable<FieldT> candidate_number;

    // Allocate variables
    voter_id.allocate(pb, "voter_id");
    candidate_number.allocate(pb, "candidate_number");

    // Add constraints
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(voter_id, 1, voter_id), "voter_id constraint");
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(candidate_number, 1, candidate_number), "candidate_number constraint");

    // Set inputs
    pb.set_input_sizes(2);

    // Generate keypairs
    r1cs_ppzksnark_keypair<default_r1cs_ppzksnark_pp> keypair = r1cs_ppzksnark_generator<default_r1cs_ppzksnark_pp>(pb.get_constraint_system());
    proving_key = keypair.pk;
    verification_key = keypair.vk;
}



void ZKPVoting::generate_and_OutputProof(const string &nickname, const string &candidate) {
    typedef Fr<default_r1cs_ppzksnark_pp> FieldT;

    // Create a protoboard
    protoboard<FieldT> pb;

    // Declare and allocate variables
    pb_variable<FieldT> voter_id;
    pb_variable<FieldT> candidate_number;
    voter_id.allocate(pb, "voter_id");
    candidate_number.allocate(pb, "candidate_number");

    // Set values for the variables
    pb.val(voter_id) = FieldT(nickname.c_str());
    pb.val(candidate_number) = FieldT(candidate.c_str());

    // Generate proof
    const r1cs_ppzksnark_proof<default_r1cs_ppzksnark_pp> proof = r1cs_ppzksnark_prover(proving_key, pb.primary_input(), pb.auxiliary_input());

    // Output the proof to a file (proof_output.txt)
    ofstream proof_output_file("proof_output.txt");
    proof_output_file << proof;
    proof_output_file.close();

    // Output the verification key to a file (vk.txt)
    ofstream vk_file("vk.txt");
    vk_file << verification_key;
    vk_file.close();
}

bool ZKPVoting::verify_Proof(const string &nickname, const string &candidate) {
    typedef Fr<default_r1cs_ppzksnark_pp> FieldT;

    // Create primary input
    r1cs_primary_input<FieldT> primary_input = {FieldT(nickname.c_str()), FieldT(candidate.c_str())};

    // Read the proof from a file (proof_output.txt)
    ifstream proof_output_file("proof_output.txt");
    r1cs_ppzksnark_proof<default_r1cs_ppzksnark_pp> proof;
    proof_output_file >> proof;
    proof_output_file.close();

    // Verify the proof
    return r1cs_ppzksnark_verifier_strong_IC(verification_key, primary_input, proof);
}

// Update whether the vote is Alice's or Bob's
void ZKPVoting::updateResults(const string &candidate) {
    if (candidate == "Alice") {
        votesAlice++;
    } else if (candidate == "Bob") {
        votesBob++;
    }
}

// Get the votes results
int ZKPVoting::get_VotesAlice() const {
    return votesAlice;
}

int ZKPVoting::get_VotesBob() const {
    return votesBob;
}
