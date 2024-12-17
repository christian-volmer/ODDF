/*

    ODDF - Open Digital Design Framework
    Copyright Advantest Corporation

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

/*

    <no description>

*/

#include "../SimulatorCore.h"
#include "../SimulatorBlockInternals.h"

#include <oddf/Exception.h>

#include <cassert>
#include <vector>

namespace oddf::simulator::common::backend {

void SimulatorCore::GenerateCode()
{
	class CodeGenerationContext : public virtual ISimulatorCodeGenerationContext {

	private:

		// No-Operation (NOP) instruction used to fulfil alignment requirements.
		struct I_NoOperation : public SimulatorInstructionBase {

		private:

			static size_t InstructionFunction(I_NoOperation &instruction)
			{
				return sizeof(instruction);
			}

		public:

			I_NoOperation(ISimulatorCodeGenerationContext &) :
				SimulatorInstructionBase(&InstructionFunction) { }
		};

	private:

		// Reference to the component managed by this instance.
		SimulatorComponent &m_component;

		std::vector<char> &m_code;

		SimulatorBlockBase *m_currentBlock;
		void *m_currentInstruction;
		size_t m_currentInstructionSize;

		CodeGenerationContext(CodeGenerationContext const &) = delete;
		void operator=(CodeGenerationContext const &) = delete;

		virtual void *InternalEmitInstruction(size_t size, size_t alignment) override
		{
			if (m_currentInstruction)
				throw Exception(ExceptionCode::IllegalMethodCall, "EmitInstruction() was already called on this block. Cannot call it a second time.");

			// Insert no-operation instructions to fulfil requested alignment
			auto misalignment = m_code.size() % alignment;
			while (misalignment) {

				EmitInstruction<I_NoOperation>();
				misalignment = m_code.size() % alignment;
			}

			m_code.insert(m_code.end(), size, 0);

			m_currentInstruction = m_code.data() + (m_code.size() - size);
			m_currentInstructionSize = size;

			return m_currentInstruction;
		}

		//
		// Input registration
		//

		void InternalRegisterInput(size_t index, void const **inputPointerPointer, design::NodeType::TypeId expectedTypeId)
		{
			if (!m_currentInstruction)
				throw Exception(ExceptionCode::IllegalMethodCall, "EmitInstruction() must be called before calling this function.");

			auto &inputs = m_currentBlock->m_internals->m_inputs;

			if (index >= inputs.size())
				throw Exception(ExceptionCode::InvalidArgument, "Parameter 'index' is out of range.");

			auto &input = inputs[index];

			if (input.m_inputPointerReference)
				throw Exception(ExceptionCode::IllegalMethodCall, "Function was already called on this input. Cannot call it a second time.");

			if (input.GetType().GetTypeId() != expectedTypeId)
				throw Exception(ExceptionCode::InvalidArgument, "Type of the argument must match the type of the simulator block input.");

			ptrdiff_t offset = (char const *)inputPointerPointer - (char const *)m_currentInstruction;

			if (offset < (ptrdiff_t)sizeof(SimulatorInstructionBase) || offset + (ptrdiff_t)sizeof(void *) > (ptrdiff_t)m_currentInstructionSize)
				throw Exception(ExceptionCode::InvalidArgument, "Argument 'inputPointerPointer' is not within the memory bounds of the current instruction.");

			input.m_inputPointerReference = reinterpret_cast<char const *>(inputPointerPointer) - m_code.data();
		}

		virtual void RegisterInput(size_t index, types::Boolean const *&inputPointerReference) override
		{
			InternalRegisterInput(index, reinterpret_cast<void const **>(&inputPointerReference), design::NodeType::BOOLEAN);
		}

		//
		// Output registration
		//

		void InternalRegisterOutput(size_t index, void *storagePointer, size_t storageSize, design::NodeType::TypeId expectedTypeId)
		{
			if (!m_currentInstruction)
				throw Exception(ExceptionCode::IllegalMethodCall, "EmitInstruction() must be called before calling this function.");

			auto &outputs = m_currentBlock->m_internals->m_outputs;

			if (index >= outputs.size())
				throw Exception(ExceptionCode::InvalidArgument, "Parameter 'index' is out of range.");

			auto &output = outputs[index];

			if (output.m_storageReference)
				throw Exception(ExceptionCode::IllegalMethodCall, "Function was already called on this output. Cannot call it a second time.");

			if (output.GetType().GetTypeId() != expectedTypeId)
				throw Exception(ExceptionCode::InvalidArgument, "Type of the argument must match the type of the simulator block output.");

			if (types::GetStoredByteSize(output.GetType()) != storageSize)
				throw Exception(ExceptionCode::InvalidArgument, "Parameter 'storageSize' does not match the type of the simulator block output.");

			ptrdiff_t offset = (char const *)storagePointer - (char const *)m_currentInstruction;

			if (offset < (ptrdiff_t)sizeof(SimulatorInstructionBase) || offset + (ptrdiff_t)storageSize > (ptrdiff_t)m_currentInstructionSize)
				throw Exception(ExceptionCode::InvalidArgument, "Arguments 'storagePointer' and 'storageSize' are not within the memory bounds of the current instruction.");

			output.m_storageReference = reinterpret_cast<char const *>(storagePointer) - m_code.data();
		}

		virtual void RegisterOutput(size_t index, types::Boolean &outputReference) override
		{
			InternalRegisterOutput(index, &outputReference, sizeof(types::Boolean), design::NodeType::BOOLEAN);
		}

		virtual void RegisterOutput(size_t index, types::FixedPointElement *outputReference, size_t elementCount) override
		{
			InternalRegisterOutput(index, outputReference, sizeof(types::FixedPointElement) * elementCount, design::NodeType::FIXED_POINT);
		}

		void TranslateOutputReferences()
		{
			for (auto *block : m_component.m_blocks) {

				for (auto &output : block->m_internals->m_outputs)
					output.m_storagePointer = m_code.data() + output.m_storageReference;
			}
		}

		void TranslateInputReferences()
		{
			for (auto *block : m_component.m_blocks) {

				for (auto &input : block->m_internals->m_inputs) {

					if (input.m_inputPointerReference)
						*reinterpret_cast<void const **>(m_code.data() + input.m_inputPointerReference) = input.m_driver->m_storagePointer;
				}
			}
		}

		//
		// ISimulatorComponentContext
		//

		virtual void RegisterGlobalObject(std::string name, std::unique_ptr<IObject> &&object) override
		{
			m_component.m_simulatorCore.RegisterGlobalObject(name, std::move(object));
		}

		virtual ISimulatorComponent &GetCurrentComponent() noexcept override
		{
			return m_component;
		}

		virtual void RegisterComponentObject(Uid const &clsid, std::unique_ptr<IObject> &&object) override
		{
			m_component.RegisterComponentObject(clsid, std::move(object));
		}

		virtual void *GetComponentObject(Uid const &clsid, Uid const &iid) const override
		{
			return m_component.GetComponentObject(clsid, iid);
		}

	public:

		CodeGenerationContext(SimulatorComponent &component) :
			m_component(component),
			m_code(component.m_code),
			m_currentBlock(nullptr),
			m_currentInstruction(nullptr),
			m_currentInstructionSize()
		{
		}

		void GenerateCode()
		{
			for (auto *block : m_component.m_blocks) {

				assert(block);
				m_currentBlock = block;
				m_currentInstruction = nullptr;
				m_currentInstructionSize = 0;

				m_currentBlock->GenerateCode(*this);

				if (m_currentInstruction) {

					for (auto const &input : m_currentBlock->m_internals->m_inputs)
						if (!input.m_inputPointerReference)
							throw Exception(ExceptionCode::Unexpected, "Block failed to register at least one of its inputs.");
				}

				for (auto const &output : m_currentBlock->m_internals->m_outputs)
					if (!output.m_storageReference)
						throw Exception(ExceptionCode::Unexpected, "Block failed to register at least one of its outputs.");
			}

			m_code.shrink_to_fit();

			TranslateOutputReferences();
			TranslateInputReferences();
		}
	};

	for (auto &component : m_components) {

		auto codeGenerationContext = CodeGenerationContext(component);
		codeGenerationContext.GenerateCode();
	}
}

} // namespace oddf::simulator::common::backend
