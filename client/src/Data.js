export const BACKWARD = 'backward';
export const FORWARD = 'forward';
export const LEFT = 'leftfwd';
export const RIGHT = 'rightfwd';
export const PIVRIGHT = 'right';
export const PIVLEFT = 'left';
export const LOOKING = 'looking';
export const GATHER = 'gather';
export const WATCH = 'watch';
export const ATTACK = 'attack';
export const SELFID = 'selfid';
export const SELFSTATS = 'selfstats';
export const INSPECT = 'inspect';
export const NEXT = 'next';
export const JUMP = 'jump';
export const IDENTIFY = 'identify';
export const TIMEOUT = 30000;

export const POSSIBLE_PATH = [
  [FORWARD],
  [FORWARD, FORWARD, LEFT],
  [FORWARD, FORWARD],
  [FORWARD, FORWARD, RIGHT],
];

export const randomMoves = [
  FORWARD,
  FORWARD,
  BACKWARD,
  LEFT,
  LEFT,
  RIGHT,
];
