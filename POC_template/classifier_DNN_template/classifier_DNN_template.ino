#include <TensorFlowLite_ESP32.h>
// tensorflowlite_esp32
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// TODO: PUT YOUR MODEL FILE NAME HERE
// This file needs to be in the sketch folder
#include "___.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
constexpr int kTensorArenaSize = 3000;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

void setup() {
  Serial.begin(115200);

  // Set up logging
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure
  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);
}

void loop() {
  // TODO: PUT TEST INPUT VALUES HERE
  // Set model input
  float db1 = /**/;
  float db2 = /**/;
  
  input->data.f[0] = db1;
  input->data.f[1] = db2;

  // Run inference
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on db1: %f\n",
                         static_cast<double>(db1));
    return;
  }

  // Get model output
  float* y = output->data.f;

  // Output the results.
  Serial.printf("%f, %f, %f\n",y[0] ,y[1] ,y[2]);
  delay(500);
}
