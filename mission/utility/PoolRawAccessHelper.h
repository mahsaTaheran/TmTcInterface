#ifndef MISSION_UTILITY_POOLRAWACCESSHELPER_H_
#define MISSION_UTILITY_POOLRAWACCESSHELPER_H_

#include <fsfw/returnvalues/HasReturnvaluesIF.h>
#include <fsfw/datapool/DataSet.h>
#include <fsfw/datapool/PoolRawAccess.h>

/**
 * @brief 	This helper function simplifies accessing data pool entries
 *  	  	via PoolRawAccess
 * @details Can be used for a Housekeeping Service
 * 			like  ECSS PUS Service 3 if the type of the datapool entries is unknown.
 * 			The provided dataset can be serialized into a provided buffer automatically by
 * 			providing a buffer of pool IDs
 * @ingroup data_pool
 */
class PoolRawAccessHelper: public HasReturnvaluesIF {
public:
	/**
	 * Call this constructor if a dataset needs to be serialized via
	 * Pool Raw Access
	 * @param dataSet_ This dataset will be used to perform thread-safe reading
	 * @param poolIdBuffer_ A buffer of uint32_t pool IDs
	 * @param numberOfParameters_ The number of parameters / pool IDs
	 */
	PoolRawAccessHelper(uint32_t * poolIdBuffer_, uint8_t numberOfParameters_);
	virtual ~PoolRawAccessHelper();

	/**
	 * Serialize the datapool entries derived from the pool ID buffer
	 * directly into a provided buffer
	 * @param [out] buffer
	 * @param [out] size Size of the serialized buffer
	 * @param max_size
	 * @param bigEndian
	 * @return 	@c RETURN_OK On success
	 * 			@c RETURN_FAILED on failure
	 */
	ReturnValue_t serialize(uint8_t ** buffer, size_t * size,
			const size_t max_size, SerializeIF::Endianness streamEndianness);

	/**
	 * Serializes data pool entries into provided buffer with the validity mask buffer
	 * at the end of the buffer. Every bit of the validity mask denotes
	 * the validity of a corresponding data pool entry from left to right.
	 * @param [out] buffer
	 * @param [out] size Size of the serialized buffer plus size
	 *                   of the validity mask
	 * @return 	@c RETURN_OK On success
	 * 			@c RETURN_FAILED on failure
	 */
	ReturnValue_t serializeWithValidityMask(uint8_t ** buffer, size_t * size,
			const size_t max_size, SerializeIF::Endianness streamEndianness);


private:
	// DataSet * dataSet;
	const uint8_t * poolIdBuffer;
	uint8_t numberOfParameters;
	static const uint8_t SET_MAX_SIZE = 63;
	uint8_t validBufferIndex;
	uint8_t validBufferIndexBit;

	struct SerializationArgs {
		uint8_t ** buffer;
		size_t * size;
		const size_t max_size;
		SerializeIF::Endianness streamEndianness;
	};
	/**
	 * Helper function to serialize single pool entries
	 * @param pPoolIdBuffer
	 * @param buffer
	 * @param remainingParameters
	 * @param hkDataSize
	 * @param max_size
	 * @param bigEndian
	 * @param withValidMask Can be set optionally to set a
	 *        provided validity mask
	 * @param validityMask Can be supplied and will be set if
	 *  	  @c withValidMask is set to true
	 * @return
	 */
	ReturnValue_t serializeCurrentPoolEntryIntoBuffer(
			SerializationArgs argStruct, size_t * remainingParameters,
			bool withValidMask = false, uint8_t * validityMask = nullptr);

	ReturnValue_t handlePoolEntrySerialization(uint32_t currentPoolId,
			SerializationArgs argStruct, bool withValidMask = false,
			uint8_t * validityMask = nullptr);

	ReturnValue_t checkRemainingSize(PoolRawAccess * currentPoolRawAccess,
			bool * isSerialized, uint8_t * arrayPosition);
	void handleMaskModification(uint8_t * validityMask);
	/**
	 * Sets specific bit of a byte
	 * @param byte
	 * @param position Position of byte to set from 1 to 8
	 * @param value Binary value to set
	 * @return
	 */
	uint8_t bitSetter(uint8_t byte, uint8_t position, bool value);
};

#endif /* FRAMEWORK_DATAPOOL_POOLRAWACCESSHELPER_H_ */
