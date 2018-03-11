// Copyright (c) 2012-2018, The CryptoNote developers, YxomTech
//
// This file is part of Varcoin.
//
// Varcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Varcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Varcoin.  If not, see <http://www.gnu.org/licenses/>.

#include "ICoreStub.h"

#include "VarNoteCore/VarNoteFormatUtils.h"
#include "VarNoteCore/VarNoteTools.h"
#include "VarNoteCore/IBlock.h"
#include "VarNoteCore/VerificationContext.h"


ICoreStub::ICoreStub() :
    topHeight(0),
    globalIndicesResult(false),
    randomOutsResult(false),
    poolTxVerificationResult(true),
    poolChangesResult(true) {
}

ICoreStub::ICoreStub(const VarNote::Block& genesisBlock) :
    topHeight(0),
    globalIndicesResult(false),
    randomOutsResult(false),
    poolTxVerificationResult(true),
    poolChangesResult(true) {
  addBlock(genesisBlock);
}

bool ICoreStub::addObserver(VarNote::ICoreObserver* observer) {
  return true;
}

bool ICoreStub::removeObserver(VarNote::ICoreObserver* observer) {
  return true;
}

void ICoreStub::get_blockchain_top(uint32_t& height, Crypto::Hash& top_id) {
  height = topHeight;
  top_id = topId;
}

std::vector<Crypto::Hash> ICoreStub::findBlockchainSupplement(const std::vector<Crypto::Hash>& remoteBlockIds, size_t maxCount,
  uint32_t& totalBlockCount, uint32_t& startBlockIndex) {

  //Sending all blockchain
  totalBlockCount = static_cast<uint32_t>(blocks.size());
  startBlockIndex = 0;
  std::vector<Crypto::Hash> result;
  result.reserve(std::min(blocks.size(), maxCount));
  for (uint32_t height = 0; height < static_cast<uint32_t>(std::min(blocks.size(), maxCount)); ++height) {
    assert(blockHashByHeightIndex.count(height) > 0);
    result.push_back(blockHashByHeightIndex[height]);
  }
  return result;
}

bool ICoreStub::get_random_outs_for_amounts(const VarNote::COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS_request& req,
    VarNote::COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS_response& res) {
  res = randomOuts;
  return randomOutsResult;
}

bool ICoreStub::get_tx_outputs_gindexs(const Crypto::Hash& tx_id, std::vector<uint32_t>& indexs) {
  std::copy(globalIndices.begin(), globalIndices.end(), std::back_inserter(indexs));
  return globalIndicesResult;
}

VarNote::i_varnote_protocol* ICoreStub::get_protocol() {
  return nullptr;
}

bool ICoreStub::handle_incoming_tx(VarNote::BinaryArray const& tx_blob, VarNote::tx_verification_context& tvc, bool keeped_by_block) {
  return true;
}

void ICoreStub::set_blockchain_top(uint32_t height, const Crypto::Hash& top_id) {
  topHeight = height;
  topId = top_id;
}

void ICoreStub::set_outputs_gindexs(const std::vector<uint32_t>& indexs, bool result) {
  globalIndices.clear();
  std::copy(indexs.begin(), indexs.end(), std::back_inserter(globalIndices));
  globalIndicesResult = result;
}

void ICoreStub::set_random_outs(const VarNote::COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS_response& resp, bool result) {
  randomOuts = resp;
  randomOutsResult = result;
}

std::vector<VarNote::Transaction> ICoreStub::getPoolTransactions() {
  return std::vector<VarNote::Transaction>();
}

bool ICoreStub::getPoolChanges(const Crypto::Hash& tailBlockId, const std::vector<Crypto::Hash>& knownTxsIds,
                               std::vector<VarNote::Transaction>& addedTxs, std::vector<Crypto::Hash>& deletedTxsIds) {
  std::unordered_set<Crypto::Hash> knownSet;
  for (const Crypto::Hash& txId : knownTxsIds) {
    if (transactionPool.find(txId) == transactionPool.end()) {
      deletedTxsIds.push_back(txId);
    }

    knownSet.insert(txId);
  }

  for (const std::pair<Crypto::Hash, VarNote::Transaction>& poolEntry : transactionPool) {
    if (knownSet.find(poolEntry.first) == knownSet.end()) {
      addedTxs.push_back(poolEntry.second);
    }
  }

  return poolChangesResult;
}

bool ICoreStub::getPoolChangesLite(const Crypto::Hash& tailBlockId, const std::vector<Crypto::Hash>& knownTxsIds,
        std::vector<VarNote::TransactionPrefixInfo>& addedTxs, std::vector<Crypto::Hash>& deletedTxsIds) {
  std::vector<VarNote::Transaction> added;
  bool returnStatus = getPoolChanges(tailBlockId, knownTxsIds, added, deletedTxsIds);

  for (const auto& tx : added) {
    VarNote::TransactionPrefixInfo tpi;
    tpi.txPrefix = tx;
    tpi.txHash = getObjectHash(tx);

    addedTxs.push_back(std::move(tpi));
  }

  return returnStatus;
}

void ICoreStub::getPoolChanges(const std::vector<Crypto::Hash>& knownTxsIds, std::vector<VarNote::Transaction>& addedTxs,
                               std::vector<Crypto::Hash>& deletedTxsIds) {
}

bool ICoreStub::queryBlocks(const std::vector<Crypto::Hash>& block_ids, uint64_t timestamp,
  uint32_t& start_height, uint32_t& current_height, uint32_t& full_offset, std::vector<VarNote::BlockFullInfo>& entries) {
  //stub
  return true;
}

bool ICoreStub::queryBlocksLite(const std::vector<Crypto::Hash>& block_ids, uint64_t timestamp,
  uint32_t& start_height, uint32_t& current_height, uint32_t& full_offset, std::vector<VarNote::BlockShortInfo>& entries) {
  //stub
  return true;
}

std::vector<Crypto::Hash> ICoreStub::buildSparseChain() {
  std::vector<Crypto::Hash> result;
  result.reserve(blockHashByHeightIndex.size());
  for (auto kvPair : blockHashByHeightIndex) {
    result.emplace_back(kvPair.second);
  }

  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<Crypto::Hash> ICoreStub::buildSparseChain(const Crypto::Hash& startBlockId) {
  // TODO implement
  assert(blocks.count(startBlockId) > 0);
  std::vector<Crypto::Hash> result;
  result.emplace_back(blockHashByHeightIndex[0]);
  return result;
}

size_t ICoreStub::addChain(const std::vector<const VarNote::IBlock*>& chain) {
  size_t blocksCounter = 0;
  for (const VarNote::IBlock* block : chain) {
    for (size_t txNumber = 0; txNumber < block->getTransactionCount(); ++txNumber) {
      const VarNote::Transaction& tx = block->getTransaction(txNumber);
      Crypto::Hash txHash = VarNote::NULL_HASH;
      size_t blobSize = 0;
      getObjectHash(tx, txHash, blobSize);
      addTransaction(tx);
    }
    addBlock(block->getBlock());
    ++blocksCounter;
  }

  return blocksCounter;
}

Crypto::Hash ICoreStub::getBlockIdByHeight(uint32_t height) {
  auto iter = blockHashByHeightIndex.find(height);
  if (iter == blockHashByHeightIndex.end()) {
    return VarNote::NULL_HASH;
  }
  return iter->second;
}

bool ICoreStub::getBlockByHash(const Crypto::Hash &h, VarNote::Block &blk) {
  auto iter = blocks.find(h);
  if (iter == blocks.end()) {
    return false;
  }
  blk = iter->second;
  return true;
}

bool ICoreStub::getBlockHeight(const Crypto::Hash& blockId, uint32_t& blockHeight) {
  auto it = blocks.find(blockId);
  if (it == blocks.end()) {
    return false;
  }
  blockHeight = get_block_height(it->second);
  return true;
}

void ICoreStub::getTransactions(const std::vector<Crypto::Hash>& txs_ids, std::list<VarNote::Transaction>& txs, std::list<Crypto::Hash>& missed_txs, bool checkTxPool) {
  for (const Crypto::Hash& hash : txs_ids) {
    auto iter = transactions.find(hash);
    if (iter != transactions.end()) {
      txs.push_back(iter->second);
    } else {
      missed_txs.push_back(hash);
    }
  }
  if (checkTxPool) {
    std::list<Crypto::Hash> pullTxIds(std::move(missed_txs));
    missed_txs.clear();
    for (const Crypto::Hash& hash : pullTxIds) {
      auto iter = transactionPool.find(hash);
      if (iter != transactionPool.end()) {
        txs.push_back(iter->second);
      }
      else {
        missed_txs.push_back(hash);
      }
    }
  }
}

bool ICoreStub::getBackwardBlocksSizes(uint32_t fromHeight, std::vector<size_t>& sizes, size_t count) {
  return true;
}

bool ICoreStub::getBlockSize(const Crypto::Hash& hash, size_t& size) {
  return true;
}

bool ICoreStub::getAlreadyGeneratedCoins(const Crypto::Hash& hash, uint64_t& generatedCoins) {
  return true;
}

bool ICoreStub::getBlockReward(size_t medianSize, size_t currentBlockSize, uint64_t alreadyGeneratedCoins, uint64_t fee,
    bool penalizeFee, uint64_t& reward, int64_t& emissionChange) {
  return true;
}

bool ICoreStub::scanOutputkeysForIndices(const VarNote::KeyInput& txInToKey, std::list<std::pair<Crypto::Hash, size_t>>& outputReferences) {
  return true;
}

bool ICoreStub::getBlockDifficulty(uint32_t height, VarNote::difficulty_type& difficulty) {
  return true;
}

bool ICoreStub::getBlockContainingTx(const Crypto::Hash& txId, Crypto::Hash& blockId, uint32_t& blockHeight) {
  auto iter = blockHashByTxHashIndex.find(txId);
  if (iter == blockHashByTxHashIndex.end()) {
    return false;
  }
  blockId = iter->second;
  auto blockIter = blocks.find(blockId);
  if (blockIter == blocks.end()) {
    return false;
  }
  blockHeight = boost::get<VarNote::BaseInput>(blockIter->second.baseTransaction.inputs.front()).blockIndex;
  return true;
}

bool ICoreStub::getMultisigOutputReference(const VarNote::MultisignatureInput& txInMultisig, std::pair<Crypto::Hash, size_t>& outputReference) {
  return true;
}

void ICoreStub::addBlock(const VarNote::Block& block) {
  uint32_t height = boost::get<VarNote::BaseInput>(block.baseTransaction.inputs.front()).blockIndex;
  Crypto::Hash hash = VarNote::get_block_hash(block);
  if (height > topHeight) {
    topHeight = height;
    topId = hash;
  }
  blocks.emplace(std::make_pair(hash, block));
  blockHashByHeightIndex.emplace(std::make_pair(height, hash));

  blockHashByTxHashIndex.emplace(std::make_pair(VarNote::getObjectHash(block.baseTransaction), hash));
  for (auto txHash : block.transactionHashes) {
    blockHashByTxHashIndex.emplace(std::make_pair(txHash, hash));
  }
}

void ICoreStub::addTransaction(const VarNote::Transaction& tx) {
  Crypto::Hash hash = VarNote::getObjectHash(tx);
  transactions.emplace(std::make_pair(hash, tx));
}

bool ICoreStub::getGeneratedTransactionsNumber(uint32_t height, uint64_t& generatedTransactions) {
  return true;
}

bool ICoreStub::getOrphanBlocksByHeight(uint32_t height, std::vector<VarNote::Block>& blocks) {
  return true;
}

bool ICoreStub::getBlocksByTimestamp(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t blocksNumberLimit, std::vector<VarNote::Block>& blocks, uint32_t& blocksNumberWithinTimestamps) {
  return true;
}

bool ICoreStub::getPoolTransactionsByTimestamp(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t transactionsNumberLimit, std::vector<VarNote::Transaction>& transactions, uint64_t& transactionsNumberWithinTimestamps) {
  return true;
}

bool ICoreStub::getTransactionsByPaymentId(const Crypto::Hash& paymentId, std::vector<VarNote::Transaction>& transactions) {
  return true;
}

std::error_code ICoreStub::executeLocked(const std::function<std::error_code()>& func) {
  return func();
}

std::unique_ptr<VarNote::IBlock> ICoreStub::getBlock(const Crypto::Hash& blockId) {
  return std::unique_ptr<VarNote::IBlock>(nullptr);
}

bool ICoreStub::handleIncomingTransaction(const VarNote::Transaction& tx, const Crypto::Hash& txHash, size_t blobSize, VarNote::tx_verification_context& tvc, bool keptByBlock) {
  auto result = transactionPool.emplace(std::make_pair(txHash, tx));
  tvc.m_verifivation_failed = !poolTxVerificationResult;
  tvc.m_added_to_pool = true;
  tvc.m_should_be_relayed = result.second;
  return poolTxVerificationResult;
}

bool ICoreStub::have_block(const Crypto::Hash& id) {
  return blocks.count(id) > 0;
}

void ICoreStub::setPoolTxVerificationResult(bool result) {
  poolTxVerificationResult = result;
}

bool ICoreStub::addMessageQueue(VarNote::MessageQueue<VarNote::BlockchainMessage>& messageQueuePtr) {
  return true;
}

bool ICoreStub::removeMessageQueue(VarNote::MessageQueue<VarNote::BlockchainMessage>& messageQueuePtr) {
  return true;
}

void ICoreStub::setPoolChangesResult(bool result) {
  poolChangesResult = result;
}