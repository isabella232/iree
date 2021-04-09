# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from absl import app
from iree.tf.support import tf_test_utils
import tensorflow as tf


# Empty lists and dicts are currently unsupported. IREE also currently cannot
# represent multiple sequence types, so we turn all sequences into tuples.
class PyTreeModule(tf_test_utils.TestModule):

  @tf_test_utils.tf_function_unit_test(input_signature=[])
  def output_tuple_len_1(self):
    return (0,)

  @tf_test_utils.tf_function_unit_test(input_signature=[])
  def output_tuple_len_2(self):
    return 0, 1

  @tf_test_utils.tf_function_unit_test(input_signature=[])
  def output_tuple_len_3(self):
    return 0, 1, 2

  @tf_test_utils.tf_function_unit_test(input_signature=[])
  def output_nested_pytree(self):
    return {"key_a": (0, 1, 2), "key_b": (0, 1, {"key_c": (0, 1)})}

  @tf_test_utils.tf_function_unit_test(input_signature=[{
      "key_a": (tf.TensorSpec([]), tf.TensorSpec([]), tf.TensorSpec([])),
      "key_b": (tf.TensorSpec([]), tf.TensorSpec([]), {
          "key_c": (tf.TensorSpec([]), tf.TensorSpec([]))
      })
  }])
  def input_nested_pytree(self, input_pytree):
    return input_pytree


class PyTreeTest(tf_test_utils.TracedModuleTestCase):

  def __init__(self, *args, **kwargs):
    super().__init__(*args, **kwargs)
    self._modules = tf_test_utils.compile_tf_module(PyTreeModule)


def main(argv):
  del argv  # Unused
  PyTreeTest.generate_unit_tests(PyTreeModule)
  tf.test.main()


if __name__ == '__main__':
  app.run(main)