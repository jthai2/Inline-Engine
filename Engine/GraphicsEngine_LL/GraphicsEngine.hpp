#pragma once

#include "Pipeline.hpp"
#include "Scheduler.hpp"
#include "CommandAllocatorPool.hpp"
#include "ScratchSpacePool.hpp"
#include "ResourceResidencyQueue.hpp"
#include "PipelineEventDispatcher.hpp"
#include "PipelineEventListener.hpp"

#include "ResourceHeap.hpp"
#include "MemoryManager.hpp"
#include "HighLevelDescHeap.hpp"

#include <GraphicsApi_LL/IGxapiManager.hpp>
#include <GraphicsApi_LL/IGraphicsApi.hpp>
#include <GraphicsApi_LL/ISwapChain.hpp>
#include <GraphicsApi_LL/ICommandQueue.hpp>

#include <BaseLibrary/Logging_All.hpp>


namespace inl {
namespace gxeng {


class Mesh;
class Scene;
class MeshEntity;


struct GraphicsEngineDesc {
	gxapi::IGxapiManager* gxapiManager;
	gxapi::IGraphicsApi* graphicsApi;
	gxapi::NativeWindowHandle targetWindow;
	bool fullScreen;
	int width;
	int height;
	exc::Logger* logger;
};


// Temporary, delete this!!!!44n�gy
class PipelineEventPrinter : public PipelineEventListener {
public:
	PipelineEventPrinter() : m_log(nullptr) {}

	void SetLog(exc::LogStream* log) { m_log = log; }

	void OnFrameBeginDevice(uint64_t frameId) override {
		m_log->Event(exc::Event{ "Frame begin - DEVICE", exc::EventParameterInt("frameId", frameId) });
	}
	void OnFrameBeginHost(uint64_t frameId) override {
		m_log->Event(exc::Event{ "Frame begin - HOST", exc::EventParameterInt("frameId", frameId) });
	}
	void OnFrameCompleteDevice(uint64_t frameId) override {
		m_log->Event(exc::Event{ "Frame finished - DEVICE", exc::EventParameterInt("frameId", frameId) });
	}
	void OnFrameCompleteHost(uint64_t frameId) override {
		m_log->Event(exc::Event{ "Frame finished - HOST", exc::EventParameterInt("frameId", frameId) });
	}
private:
	exc::LogStream* m_log;
};


class GraphicsEngine {
public:
	// Custructors
	GraphicsEngine(GraphicsEngineDesc desc);
	GraphicsEngine(const GraphicsEngine&) = delete;
	GraphicsEngine& operator=(const GraphicsEngine&) = delete;
	~GraphicsEngine();

	// Update scene
	void Update(float elapsed);

	// Resources
	Mesh* CreateMesh();

	// Scene
	Scene* CreateScene(std::string name);
	MeshEntity* CreateMeshEntity();

private:
	void CreatePipeline();
private:
	// Graphics API things
	gxapi::IGxapiManager* m_gxapiManager; // external resource, we should not delete it
	gxapi::IGraphicsApi* m_graphicsApi; // external resource, we should not delete it
	std::unique_ptr<gxapi::ISwapChain> m_swapChain;

	// Pipeline Facilities
	CommandAllocatorPool m_commandAllocatorPool;
	ScratchSpacePool m_scratchSpacePool;
	Pipeline m_pipeline;
	Scheduler m_scheduler;
	std::vector<SyncPoint> m_frameEndFenceValues;

	// Memory
	HighLevelDescHeap m_descriptorHeap;
	MemoryManager m_memoryManager;
	std::unique_ptr<BackBufferHeap> m_backBufferHeap;

	// Pipeline elements
	CommandQueue m_masterCommandQueue;
	ResourceResidencyQueue m_residencyQueue;
	PipelineEventDispatcher m_pipelineEventDispatcher;
	PipelineEventPrinter m_pipelineEventPrinter; // DELETE THIS

	// Logging
	exc::Logger* m_logger;
	exc::LogStream m_logStreamGeneral;
	exc::LogStream m_logStreamPipeline;

	// Misc
	std::chrono::nanoseconds m_absoluteTime;
	uint64_t m_frame = 0;

	// Scene
	std::set<Scene*> m_scenes;
};



} // namespace gxeng
} // namespace inl