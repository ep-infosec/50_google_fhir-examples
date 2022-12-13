// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "google/fhir/r4/json_format.h"
#include "google/fhir/r4/profiles.h"
#include "google/fhir/r4/resource_validation.h"
#include "google/fhir/status/status.h"
#include "proto/google/fhir/proto/r4/core/resources/patient.pb.h"
#include "proto/google/fhir/proto/r4/uscore.pb.h"
#include "google/fhir_examples/example_utils.h"

using std::string;

using ::google::fhir::r4::PrintFhirPrimitive;
using ::google::fhir::r4::core::Patient;
using ::google::fhir::r4::uscore::USCorePatientProfile;

// Example code for converting core Patient protos to USCore Patient protos.
// For instructions on setting up your workspace, see the top-level README.md
//
// To run:
// bazel build //cc:ProfilePatientsToUsCore
// bazel-bin/cc/ProfilePatientsToUsCore $WORKSPACE

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Missing workspace argument." << std::endl;
    return 1;
  }
  const std::string workspace = argv[1];

  const std::vector<Patient> patients = fhir_examples::ReadNdJsonFile<Patient>(
      absl::StrCat(workspace, "/ndjson/Patient.fhir.ndjson"));

  // Note that this is an example of how to convert a proto from one profile to
  // another.
  // If all we wanted to do was read the ndjson file into the profiled proto,
  // we could have called ReadNdJsonFile with <USCorePatientProfile> directly,
  // and the parser would have done the profiling automatically

  std::vector<USCorePatientProfile> uscore_patients;
  for (const Patient& patient : patients) {
    USCorePatientProfile uscore_patient;
    absl::Status status =
        google::fhir::ConvertToProfileR4(patient, &uscore_patient);
    if (!status.ok()) {
      std::cout << "Patient " << patient.identifier(0).value().value()
                << " is invalid for US Core profile: " << status.message()
                << std::endl;
    } else {
      std::cout << "Converted Patient " << uscore_patient.id().value()
                << std::endl;
      uscore_patients.push_back(uscore_patient);
    }
  }

  const USCorePatientProfile& example_patient = uscore_patients.front();

  std::cout << example_patient.DebugString() << std::endl;

  std::cout << "\n\n"
            << example_patient.name(0).given(0).value() << " "
            << example_patient.name(0).family().value() << " has gender: "
            << PrintFhirPrimitive(example_patient.birthsex()).value()
            << " and race: " << example_patient.race().text().value() << "\n\n";
}
