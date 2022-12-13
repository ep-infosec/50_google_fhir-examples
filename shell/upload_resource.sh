# !/bin/bash
# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This example uploads the analyic data in $WORKSPACE/analytic to BigQuery

if [[ $# -eq 0 ]] ; then
    echo 'Missing argument: scratch directory'
    exit 1
fi

# Note: This example requires a BigQuery project, and the bq cli tool to be
# installed.  See links in the README.md
bq mk fhirexamples

WORKSPACE=$1
RESOURCE_NAME=$2

echo "Uploading $RESOURCE_NAME..."
bq load \
  --source_format=NEWLINE_DELIMITED_JSON \
  --schema=$WORKSPACE/analytic/$RESOURCE_NAME.schema.json \
  fhirexamples.$RESOURCE_NAME $WORKSPACE/analytic/$RESOURCE_NAME.analytic.ndjson
