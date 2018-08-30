export const parseResponse = (response) => {
  const res = response.split('|');
  return { status: res[0], data: res[1] };
};

export const Wrapper = (callback) => {
  const that = {};
  let value;
  that.set = (v) => {
    value = v;
    callback(that);
  };
  that.get = () => value;
  return that;
};
export const parseCommand = (command, argument = null) => `${command}|${argument}`;

export const getRandomInt = max => Math.floor(Math.random() * Math.floor(max));

export const parseWatch = string => string.replace('[', '').replace(']', '').split(',');

export const rollDice = chances => getRandomInt(100) < chances;

export const wait = delay => new Promise((resolve) => {
  setTimeout(() => resolve(true), delay);
});
