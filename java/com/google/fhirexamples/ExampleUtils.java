/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.google.fhirexamples;

import static java.nio.charset.StandardCharsets.UTF_8;

import com.google.fhir.common.InvalidFhirException;
import com.google.fhir.common.JsonFormat.Parser;
import com.google.protobuf.Message;
import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.List;

public class ExampleUtils {

  public static <R extends Message> List<R> loadNdJson(R type, String filename)
      throws IOException, InvalidFhirException {
    Parser fhirParser = Parser.withDefaultTimeZone(ZoneId.of("America/Los_Angeles"));

    System.out.println("Reading " + filename);
    BufferedReader reader = Files.newBufferedReader(Paths.get(filename), UTF_8);

    List<R> result = new ArrayList<>();
    String line;
    while ((line = reader.readLine()) != null) {
      Message.Builder typeBuilder = type.newBuilderForType();
      fhirParser.merge(line, typeBuilder);
      result.add((R) typeBuilder.build());
    }
    return result;
  }

  private ExampleUtils() {}
}
