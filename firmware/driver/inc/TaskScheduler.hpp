//
// Created by Mikhail on 12.12.23.
//

#ifndef TASK_SCHEDULER_HPP
#define TASK_SCHEDULER_HPP

#include "RingBuffer.hpp"

template <class size_type, size_type C>
class TaskScheduler {
public:
	using timestamp_type = uint32_t;
	using task_type = void     (*)();
	constexpr static size_type intervalMask {1u << (sizeof(size_type) * 8 - 1)};
	constexpr static size_type delayMask {~intervalMask};

	TaskScheduler() = default;
	~TaskScheduler() = default;

	size_type setTimeout(timestamp_type currentTime, timestamp_type timeout, task_type cb);
	size_type setInterval(timestamp_type currentTime, timestamp_type interval, task_type cb);
	void      clearTimeout(size_type id);
	void      clearInterval(size_type id);

	task_type getNextTask(timestamp_type currentTime);
	void      execute(timestamp_type currentTime);

	size_type size() const;
	bool      empty() const;

	void reset();

protected:
	struct Task {
		task_type      cb {nullptr};
		timestamp_type added {0};
		timestamp_type delay {0};
		size_type      id {0};
	};

	size_type schedule(timestamp_type currentTime, timestamp_type delay, task_type cb, bool interval = false);
	void      unschedule(size_type id);

	static size_type lastID;

	RingBuffer<Task, size_type, C> _tasks {};
};


template <class size_type, size_type C>
size_type TaskScheduler<size_type, C>::lastID {0};

template <class size_type, size_type C>
size_type TaskScheduler<size_type, C>::schedule(
    timestamp_type currentTime,
    timestamp_type delay,
    task_type      cb,
    bool           interval
) {
	if (_tasks.full()) {
		return 0;
	}

	Task task {cb, currentTime, delay, interval ? static_cast<size_type>(lastID | intervalMask) : lastID};

	++lastID == intervalMask ? lastID = 0 : lastID;

	if (!_tasks.empty()) {
		size_type i {0};
		for (; task.delay - task.added >= _tasks[i].delay - _tasks[i].added && i < _tasks.size(); ++i);

		// When the position is found, shift existing tasks
		if (i < _tasks.size() / 2) {  // If the new task should be closer to the front, shift the tasks before
			_tasks.push_front(_tasks.front());
			for (size_type j {0}; j < i - 1; ++j) {
				_tasks[j + 1] = _tasks[j + 2];
			}
		} else {  // Otherwise shift the tasks after
			_tasks.push_back(_tasks.back());
			for (size_type j = _tasks.size() - 1; j > i + 1; --j) {
				_tasks[j - 1] = _tasks[j - 2];
			}
		}

		_tasks[i] = task;
	} else {
		_tasks.push_back(task);
	}

	return task.id;
}

template <class size_type, size_type C>
void TaskScheduler<size_type, C>::unschedule(size_type id) {
	size_type i {0};
	for (; _tasks[i].id != id && i < _tasks.size(); ++i);

	// Same logic as when adding tasks: shift the smaller part
	if (i < _tasks.size() / 2) {
		for (size_type j {i}; j > 0; --j) {
			_tasks[j] = _tasks[j - 1];
		}
		_tasks.pop_front();
	} else {
		for (size_type j {i}; j < _tasks.size() - 1; ++j) {
			_tasks[j] = _tasks[j + 1];
		}
		_tasks.pop_back();
	}
}

template <class size_type, size_type C>
size_type TaskScheduler<size_type, C>::setTimeout(timestamp_type currentTime, timestamp_type timeout, task_type cb) {
	return schedule(currentTime, timeout, cb);
}

template <class size_type, size_type C>
size_type TaskScheduler<size_type, C>::setInterval(timestamp_type currentTime, timestamp_type interval, task_type cb) {
	return schedule(currentTime, interval, cb, interval);
}

template <class size_type, size_type C>
void TaskScheduler<size_type, C>::clearTimeout(size_type id) {
	unschedule(id);
}

template <class size_type, size_type C>
void TaskScheduler<size_type, C>::clearInterval(size_type id) {
	unschedule(id);
}

template <class size_type, size_type C>
typename TaskScheduler<size_type, C>::task_type TaskScheduler<size_type, C>::getNextTask(timestamp_type currentTime) {
	if (_tasks.empty()) {
		return nullptr;
	}

	Task task {_tasks.front()};

	if (currentTime - task.added >= task.delay) {
		if (task.id & intervalMask) {
			schedule(currentTime, task.delay, task.cb, true);
		}
		_tasks.pop_front();
		return task.cb;
	} else {
		return nullptr;
	}
}

template <class size_type, size_type C>
void TaskScheduler<size_type, C>::execute(timestamp_type currentTime) {
	for (task_type cb {getNextTask(currentTime)}; cb; cb = getNextTask(currentTime)) {
		cb();
	}
}

template <class size_type, size_type C>
size_type TaskScheduler<size_type, C>::size() const {
	return _tasks.size();
}

template <class size_type, size_type C>
bool TaskScheduler<size_type, C>::empty() const {
	return _tasks.empty();
}

template <class size_type, size_type C>
void TaskScheduler<size_type, C>::reset() {
	_tasks.clear();
	lastID = 0;
}


#endif  // TASK_SCHEDULER_HPP
