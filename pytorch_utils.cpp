// #include "pytorch_utils.h"
// #include <torch/torch.h>

// torch::jit::script::Module load_model(const std::string& model_path) {
//     torch::jit::script::Module module;
//     try {
//         module = torch::jit::load(model_path);
//     } catch (const c10::Error& e) {
//         std::cerr << "Error loading the model\n";
//         return module;
//     }
//     return module;
// }
