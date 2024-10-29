#pragma once

class component_interface
{
public:
    virtual ~component_interface() = default;
    virtual void post_start() {}
    virtual void lua_start() {}
    virtual void pre_destroy() {}
    virtual void start_hooks() {}
    virtual void destroy_hooks() {}
};

enum class component_priority
{
	min = 0,
	// must run after the steam_proxy
	name,
	// must run after the updater
	steam_proxy,
	updater,
	// must have the highest priority
	arxan,
};

enum class component_type
{
	client,
	server,
	any,
};

struct generic_component
{
	static constexpr component_type type = component_type::any;

	virtual ~generic_component() = default;

	virtual void post_load()
	{
	}

	virtual void pre_destroy()
	{
	}

	virtual void post_unpack()
	{
	}

	virtual component_priority priority() const
	{
		return component_priority::min;
	}
};

struct client_component : generic_component
{
	static constexpr component_type type = component_type::client;
};

struct server_component : generic_component
{
	static constexpr component_type type = component_type::server;
};
